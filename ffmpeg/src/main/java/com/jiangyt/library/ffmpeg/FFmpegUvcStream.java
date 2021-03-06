package com.jiangyt.library.ffmpeg;

/**
 * 类说明：ffmpeg视频流推送
 * <p>
 * 包名： com.jiangyt.library.ffmpeg
 *
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/2/20 下午1:16
 */
public class FFmpegUvcStream {
    static {
        System.loadLibrary("ffmpeg_uvc_stream");
    }

    public native int setCallback(FrameCallback frameCallback);

    public native void startPublish(String stream, int width, int height);

    public native void stopPublish();

    public native void captureFrame();
}
