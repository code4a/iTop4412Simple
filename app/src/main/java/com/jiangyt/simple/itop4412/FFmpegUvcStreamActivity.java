package com.jiangyt.simple.itop4412;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

import com.jiangyt.library.ffmpeg.FFmpegUvcStream;
import com.jiangyt.library.ffmpeg.FrameCallback;

import java.nio.ByteBuffer;

import io.reactivex.rxjava3.schedulers.Schedulers;

public class FFmpegUvcStreamActivity extends AppCompatActivity implements View.OnClickListener {

    private Button mBtnEncodeStartMP4, mBtnEncodeStopMP4;
    private ImageView mImag;
    private Bitmap bitmap;
    FFmpegUvcStream uvcStream;
    private int width = 320;
    private int height = 240;
    private int dwidth = 640;
    private int dheight = 480;
    private byte[] mout;
    private ByteBuffer Imagbuf;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg_uvc_stream);
        uvcStream = new FFmpegUvcStream();
        setUpView();
    }

    private void setUpView() {
        mBtnEncodeStartMP4 = findViewById(R.id.btn_encode_mp4_start);
        mBtnEncodeStartMP4.setOnClickListener(this);
        mBtnEncodeStopMP4 = findViewById(R.id.btn_encode_mp4_stop);
        mBtnEncodeStopMP4.setOnClickListener(this);
        mImag = findViewById(R.id.mimg);
        bitmap = Bitmap.createBitmap(dwidth, dheight, Bitmap.Config.RGB_565);
        mout = new byte[dwidth * dheight * 2];
        Imagbuf = ByteBuffer.wrap(mout);
        uvcStream.setCallback(new FrameCallback() {
            @Override
            public void frameCallback(byte[] rgbBuf, byte[] yuvBuf) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (rgbBuf != null && rgbBuf.length > 0) {
                            System.arraycopy(rgbBuf, 0, mout, 0, rgbBuf.length);
                            bitmap.copyPixelsFromBuffer(Imagbuf);
                            mImag.setImageBitmap(bitmap);
                            Imagbuf.clear();
                        }
                    }
                });
            }
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_encode_mp4_start:
                mBtnEncodeStartMP4.setEnabled(false);
                mBtnEncodeStopMP4.setEnabled(true);
                Schedulers.newThread().scheduleDirect(new Runnable() {
                    @Override
                    public void run() {
                        uvcStream.startPublish("rtmp://10.58.238.154:9935/live/live_camera", 240, 480);
                        uvcStream.captureFrame();
                    }
                });
                break;
            case R.id.btn_encode_mp4_stop:
                mBtnEncodeStartMP4.setEnabled(true);
                mBtnEncodeStopMP4.setEnabled(false);
                Schedulers.newThread().scheduleDirect(new Runnable() {
                    @Override
                    public void run() {
                        uvcStream.stopPublish();
                    }
                });
                break;
        }
    }
}