package com.jiangyt.library.libitop;

/**
 * 类说明：usb 摄像头
 * 进入到cpp/uvc 文件夹，执行如下命令
 * javah -o itop_uvc.h -jni -classpath ../../java com.jiangyt.library.libitop.UvcCamera
 * <p>
 * 包名： com.jiangyt.simple.itop_relay
 *
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/1/28 下午2:22
 */
public class UvcCamera {

    public static native int open(int devid);

    public static native int init(int width, int height, int numbuf);

    public static native int streamon();

    public static native int dqbuf(byte[] videodata);

    public static native int yuvtorgb(byte[] yuvdata, byte[] rgbdata, int dwidth, int dheight);

    public static native int qbuf(int index);

    public static native int videoinit(byte[] filename);

    public static native int videostart(byte[] yuvdata);

    public static native int videoclose();

    public static native int streamoff();

    public static native int release();

    static {
        System.loadLibrary("itop_uvc");
    }
}
