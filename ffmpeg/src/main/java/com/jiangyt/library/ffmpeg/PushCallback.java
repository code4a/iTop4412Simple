package com.jiangyt.library.ffmpeg;

/**
 * 类说明：推送回调
 * <p>
 * 包名： com.jinmao.cloud.app.ffmpeg
 * @see {https://www.jianshu.com/p/c398754e5984}
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/2/19 上午11:22
 */
public interface PushCallback {

    /**
     * 视频回调
     * @param pts Presentation Time Stamp。PTS主要用于度量解码后的视频帧什么时候被显示出来
     * @param dts Decode Time Stamp。DTS主要是标识读入内存中的bit流在什么时候开始送入解码器中进行解码。
     *            在没有B帧存在的情况下DTS的顺序和PTS的顺序应该是一样的。
     * @param duration 时长
     * @param index 索引
     */
    void videoCallback(long pts, long dts, long duration, long index);
}
