package com.jiangyt.simple.itop4412;

import android.content.res.Configuration;
import android.hardware.Camera;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.jiangyt.library.ffmpeg.FFMpegRtmp;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import io.reactivex.rxjava3.disposables.Disposable;
import io.reactivex.rxjava3.schedulers.Schedulers;

public class RtmpActivity extends AppCompatActivity implements View.OnClickListener, SurfaceHolder.Callback {

    public static final String TAG = "FFMPEG";

    private SurfaceView surfaceView;
    private SeekBar seekBar;
    private Button startPush, stopPush, startLive, stopLive;
    private Disposable disposable;

    private int screenWidth = 640;
    private int screenHeight = 480;
    private SurfaceHolder holder;
    private Camera camera;
    boolean isPreview = false; //是否在预览中
    private String dir = "LiveRtmp";
    private String rtmpUrl = "rtmp://10.58.238.154:8935/stream/live_camera";
    // 采集到每帧数据时间
    long previewTime = 0;
    // 开始编码时间
    long encodeTime = 0;
    // 采集数量
    int count = 0;
    // 编码数量
    int encodeCount = 0;

    private ExecutorService executor = Executors.newSingleThreadExecutor();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rtmp);

//        initPushFileView();
//        initPushFileData();

        initLiveView();
        initLiveData();
    }

    private void initLiveData() {
//        mkdirs();
        FFMpegRtmp.getInstance().initVideo(rtmpUrl);
    }

    private void mkdirs() {
        File file = new File(Environment.getExternalStorageDirectory(), dir);
        if (file.exists()) {
            file.delete();
        }
        file.mkdirs();
    }

    private void initLiveView() {
        surfaceView = findViewById(R.id.surfaceView);
        holder = surfaceView.getHolder();
        holder.addCallback(this);

        seekBar = findViewById(R.id.seekBar);
        startLive = findViewById(R.id.btn_start_live);
        startLive.setOnClickListener(this);
        stopLive = findViewById(R.id.btn_stop_live);
        stopLive.setOnClickListener(this);
        stopLive.setEnabled(false);
    }

    private void initPushFileData() {
        int ret = FFMpegRtmp.getInstance().setCallback((pts, dts, duration, index) -> runOnUiThread(() -> {
            StringBuilder sb = new StringBuilder();
            sb.append("pts: ").append(pts).append("\n");
            sb.append("dts: ").append(dts).append("\n");
            sb.append("duration: ").append(duration).append("\n");
            sb.append("index: ").append(index).append("\n");
            Log.e(TAG, sb.toString());
        }));
        Log.e(TAG, "result: " + ret);
    }

    private void initPushFileView() {
        startPush = findViewById(R.id.btn_start_push);
        startPush.setOnClickListener(this);
        stopPush = findViewById(R.id.btn_stop_push);
        stopPush.setOnClickListener(this);
        stopPush.setEnabled(false);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_start_push:
                startPush.setEnabled(false);
                stopPush.setEnabled(true);
                final File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "1234.mp4");
                if (file.exists()) {
                    disposable = Schedulers.newThread().scheduleDirect(new Runnable() {
                        @Override
                        public void run() {
                            FFMpegRtmp.getInstance().pushRtmpFile(file.getAbsolutePath());
                        }
                    });
                } else {
                    Toast.makeText(RtmpActivity.this, "文件不存在", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.btn_stop_push:
                startPush.setEnabled(true);
                stopPush.setEnabled(false);
                FFMpegRtmp.getInstance().stopPushRtmp();
                releaseDisposable();
                break;
            case R.id.btn_start_live:
                if (holder != null) {
                    startPreview(holder);
                }
                break;
            case R.id.btn_stop_live:
                break;
        }
    }

    private void startPreview(SurfaceHolder surfaceHolder) {
        try {
            if (camera == null) {
                camera = Camera.open();
                if (camera != null && !isPreview) {
                    Camera.Parameters parameters = camera.getParameters();
                    for (Camera.Size size : parameters.getSupportedPictureSizes()) {
                        Log.e(TAG, size.width + " x " + size.height);
                    }
                    for (Camera.Size size : parameters.getSupportedPreviewSizes()) {
                        Log.e(TAG, size.width + " x " + size.height);
                    }

                    // 设置预览照片的大小
                    parameters.setPreviewSize(screenWidth, screenHeight);
                    // 设置帧率
                    parameters.setPreviewFpsRange(30000, 30000);
                    // 设置图片格式
//                    parameters.setPictureFormat(ImageFormat.NV21);
                    // 设置照片的大小
                    parameters.setPictureSize(screenWidth, screenHeight);
                    // 回传参数
                    camera.setParameters(parameters);
                    // 指定使用哪个surfaceView用来预览图片
                    camera.setPreviewDisplay(surfaceHolder);
                    camera.setPreviewCallback(new Camera.PreviewCallback() {
                        @Override
                        public void onPreviewFrame(byte[] data, Camera camera) {
                            long endTime = System.currentTimeMillis();
                            executor.execute(new Runnable() {
                                @Override
                                public void run() {
                                    encodeTime = System.currentTimeMillis();
                                    FFMpegRtmp.getInstance().onFrameCallback(data);
                                    Log.e(TAG, "编码第：" + (encodeCount++) + "帧，耗时：" + (System.currentTimeMillis() - encodeTime) + " " + Thread.currentThread().getName());
                                }
                            });
                            Log.e(TAG, "采样第：" + (++count) + "帧，距上一帧间隔时间：" + (endTime - previewTime) + " " + Thread.currentThread().getName());
                            previewTime = endTime;
                        }
                    });
                    checkOrientation();
                    camera.startPreview();
                    camera.autoFocus(null);
                }
                isPreview = true;
            } else {
                camera.setPreviewDisplay(surfaceHolder);
                checkOrientation();
                camera.startPreview();
            }
        } catch (Exception e) {
            e.printStackTrace();
            camera = null;
            Toast.makeText(this, "无法获取前置摄像头", Toast.LENGTH_SHORT).show();
        }
    }

    private void checkOrientation() {
        int orientation = this.getResources().getConfiguration().orientation;
        if (orientation == Configuration.ORIENTATION_PORTRAIT) {
            camera.setDisplayOrientation(90);
        }
    }

    private void releaseCamera() {
        if (camera != null) {
            camera.setPreviewCallback(null);
            camera.stopPreview();
            camera.release();
            isPreview = false;
            camera = null;
        }
    }

    private void releaseDisposable() {
        if (disposable != null && !disposable.isDisposed()) {
            disposable.dispose();
            disposable = null;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FFMpegRtmp.getInstance().close();
        releaseCamera();
        releaseDisposable();
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        startPreview(holder);
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        startPreview(holder);
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        releaseCamera();
    }
}