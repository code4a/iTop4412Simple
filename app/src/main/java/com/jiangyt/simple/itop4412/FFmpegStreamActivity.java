package com.jiangyt.simple.itop4412;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.jiangyt.simple.itop4412.camera.CameraProxy;

public class FFmpegStreamActivity extends AppCompatActivity implements View.OnClickListener {
    private static final int PERMISSION_CODE = 1000;

    private ViewGroup mRootLayer;

    private Button mBtnEncodeStartMP4, mBtnEncodeStopMP4;

    private SurfaceView mSurfaceView;

    private CameraProxy mCameraV1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg_stream);
        applyPermission();
    }

    private void applyPermission() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, PERMISSION_CODE);
        } else {
            setupView();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_CODE && grantResults != null && grantResults.length > 0) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                setupView();
            }
        }
    }

    private void setupView() {
        mRootLayer = (ViewGroup) findViewById(R.id.camera_root_layer);

        mBtnEncodeStartMP4 = (Button) findViewById(R.id.btn_encode_mp4_start);
        mBtnEncodeStartMP4.setOnClickListener(this);
        mBtnEncodeStopMP4 = (Button) findViewById(R.id.btn_encode_mp4_stop);
        mBtnEncodeStopMP4.setOnClickListener(this);

        mSurfaceView = new SurfaceView(this);
        mRootLayer.addView(mSurfaceView);
        mCameraV1 = new CameraProxy();
        mCameraV1.setPreviewView(mSurfaceView);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_encode_mp4_start:
                mBtnEncodeStartMP4.setEnabled(false);
                mBtnEncodeStopMP4.setEnabled(true);
                mCameraV1.encodeStart("rtmp://10.58.238.154:9935/live/live_camera");
                break;
            case R.id.btn_encode_mp4_stop:
                mBtnEncodeStartMP4.setEnabled(true);
                mBtnEncodeStopMP4.setEnabled(false);
                mCameraV1.encodeStop();
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mCameraV1 != null) {
            mCameraV1.onDestroy();
            mCameraV1 = null;
        }
    }

}