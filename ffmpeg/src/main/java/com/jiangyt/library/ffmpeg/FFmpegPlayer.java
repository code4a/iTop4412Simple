package com.jiangyt.library.ffmpeg;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * 类说明：FFMpeg 播放器
 * <p>
 * 进入java目录，执行如下命令：
 * javah -o ffmpeg_player.h -cp /Users/sinochem/Library/Android/sdk/platforms/android-30/android.jar:. -jni com.jinmao.cloud.app.ffmpeg.FFmpegPlayer
 * 包名： com.jinmao.cloud.app.ffmpeg
 *
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/2/18 上午10:44
 */
public class FFmpegPlayer implements SurfaceHolder.Callback {

    private static final String PUBLISH_ADDRESS = "rtmp://10.58.238.154:8935/stream/mp4live";

    static {
        System.loadLibrary("ffmpeg_player");
    }

    private SurfaceHolder surfaceHolder;

    public void setSurfaceView(SurfaceView surfaceView) {
        if (null != this.surfaceHolder) {
            this.surfaceHolder.removeCallback(this);
        }
        this.surfaceHolder = surfaceView.getHolder();
        this.surfaceHolder.addCallback(this);

    }

    @Override

    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    @Override

    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        this.surfaceHolder = surfaceHolder;
    }

    @Override

    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    public void start(String absolutePath) {
        native_start(absolutePath, surfaceHolder.getSurface());
    }

    public int startLive(String mp4Path) {
        return startPublish(mp4Path, PUBLISH_ADDRESS);
    }

    public void stopLive() {
        stopPublish();
    }

    private native void native_start(String absolutePath, Surface surface);

    private native int startPublish(String mp4Path, String stream);

    private native void stopPublish();

}
