package com.jiangyt.simple.itop4412;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.jiangyt.library.ffmpeg.FFmpegPlayer;

import java.io.File;

import io.reactivex.rxjava3.disposables.Disposable;
import io.reactivex.rxjava3.schedulers.Schedulers;

public class PlayerActivity extends AppCompatActivity {

    private SurfaceView surfaceView;
    private FFmpegPlayer fmpegPlayer;

    private Button startLive, stopLive;
    private Disposable disposable;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_player);
        surfaceView = findViewById(R.id.surfaceView);
        findViewById(R.id.btn_player).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "1234.mp4");
                if (file.exists()) {
                    fmpegPlayer.start(file.getAbsolutePath());
                } else {
                    Toast.makeText(PlayerActivity.this, "文件不存在", Toast.LENGTH_SHORT).show();
                }
            }
        });
        startLive = findViewById(R.id.btn_start_live);
        startLive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startLive.setEnabled(false);
                stopLive.setEnabled(true);
                final File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "1234.mp4");
                if (file.exists()) {
                    disposable = Schedulers.newThread().scheduleDirect(new Runnable() {
                        @Override
                        public void run() {
                            fmpegPlayer.startLive(file.getAbsolutePath());
                        }
                    });
                } else {
                    Toast.makeText(PlayerActivity.this, "文件不存在", Toast.LENGTH_SHORT).show();
                }
            }
        });
        stopLive = findViewById(R.id.btn_stop_live);
        stopLive.setEnabled(false);
        stopLive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startLive.setEnabled(true);
                stopLive.setEnabled(false);
                fmpegPlayer.stopLive();
                disposable.dispose();
            }
        });
        findViewById(R.id.btn_test_rtmp).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(PlayerActivity.this, RtmpActivity.class));
            }
        });

        fmpegPlayer = new FFmpegPlayer();
        fmpegPlayer.setSurfaceView(surfaceView);

    }

}