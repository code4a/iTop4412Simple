package com.jiangyt.library.libitop;

/**
 * Desc: 射频
 * <p>
 *
 * @author Create by sinochem on 2020/10/10
 * <p>
 * Version: 1.0.0
 */
public class ItopRfid {

    public native int open();

    public native int close();

    public native int ioCtl(int num, int en);

    public native int[] read();

    public native byte[] readCardNum();

    static {
        System.loadLibrary("itop_rfid");
    }
}
