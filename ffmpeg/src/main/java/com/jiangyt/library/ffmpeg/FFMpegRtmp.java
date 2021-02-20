package com.jiangyt.library.ffmpeg;

/**
 * 类说明：推流
 * <p>
 * 包名： com.jinmao.cloud.app.ffmpeg
 * 如何接口中设计Android的api时，进入java目录，执行如下命令：
 * javah -o ffmpeg_rtmp.h -cp /Users/sinochem/Library/Android/sdk/platforms/android-30/android.jar:. -jni com.jinmao.cloud.app.ffmpeg.FFMpegRtmp
 * 否则，进入cpp文件夹下，执行：
 * javah -o ffmpeg_rtmp.h -jni -classpath ../java com.jinmao.cloud.app.ffmpeg.FFMpegRtmp
 *
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/2/19 上午11:21
 */
public class FFMpegRtmp {

    static {
        System.loadLibrary("ffmpeg_rtmp");
    }

    private static FFMpegRtmp instance = new FFMpegRtmp();

    public static FFMpegRtmp getInstance() {
        return instance;
    }

    public native int setCallback(PushCallback pushCallback);

    public native String getAvcodecConfiguration();

    public native int pushRtmpFile(String filePath);

    public native void stopPushRtmp();

    public native int initVideo(String rtmpUrl);

    public native int onFrameCallback(byte[] buffer);

    public native int close();
}
