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
public class FFmpegStream {
    static {
        System.loadLibrary("ffmpeg_stream");
    }

    public native void startPublish(String stream, int width, int height);

    public native void stopPublish();

    public native void onPreviewFrame(byte[] yuvData, int width, int height);
}
