package com.jiangyt.library.libitop;

/**
 * Desc: 继电器
 * <p>
 *
 * @author Create by sinochem on 2020/10/10
 * <p>
 * Version: 1.0.0
 */
public class ItopRelay {

    public native int open();

    public native int close();

    public native int ioCtl(int num, int en);

    static {
        System.loadLibrary("itop_relay");
    }
}
