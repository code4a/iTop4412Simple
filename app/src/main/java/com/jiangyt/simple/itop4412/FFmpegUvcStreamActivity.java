package com.jiangyt.simple.itop4412;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

import com.jiangyt.library.ffmpeg.FFmpegUvcStream;

import io.reactivex.rxjava3.schedulers.Schedulers;

public class FFmpegUvcStreamActivity extends AppCompatActivity implements View.OnClickListener {

    private Button mBtnEncodeStartMP4, mBtnEncodeStopMP4;
    FFmpegUvcStream uvcStream;

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
                        uvcStream.startPublish("rtmp://10.58.238.36:9935/live/live_camera", 240, 480);
                        uvcStream.captureFrame();
                    }
                });
                break;
            case R.id.btn_encode_mp4_stop:
                mBtnEncodeStartMP4.setEnabled(true);
                mBtnEncodeStopMP4.setEnabled(false);
                uvcStream.stopPublish();
                break;
        }
    }
}