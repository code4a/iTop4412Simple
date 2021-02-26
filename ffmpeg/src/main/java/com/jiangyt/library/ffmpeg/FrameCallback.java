package com.jiangyt.library.ffmpeg;

/**
 * 类说明：帧数据回调接口
 * <p>
 * 包名： com.jiangyt.library.ffmpeg
 *
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/2/26 上午11:08
 */
public interface FrameCallback {

    void frameCallback(byte[] rgbBuf, byte[] yuvBuf);
}
