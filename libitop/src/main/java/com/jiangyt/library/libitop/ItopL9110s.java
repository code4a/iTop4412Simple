package com.jiangyt.library.libitop;

import androidx.annotation.IntRange;

/**
 * Desc: 继电器
 * <p>
 * 进入到cpp/l9110s 文件夹，执行如下命令
 * javah -o itop_l9110s.h -jni -classpath ../../java com.jiangyt.library.libitop.ItopL9110s
 *
 * @author Create by sinochem on 2020/10/10
 * <p>
 * Version: 1.0.0
 */
public class ItopL9110s {

    public static final int M1 = 1;
    public static final int M2 = 2;

    private static final int IA1 = 0;
    private static final int IB1 = 1;
    private static final int IA2 = 2;
    private static final int IB2 = 3;

    private static int HIGH = 1;
    private static int LOW = 0;

    public void start(boolean reverse, @IntRange(from = 1, to = 2) int motor) {
        stop(motor);
        if (reverse) {
            ioCtl(motor == M1 ? IA1 : IA2, LOW);
            ioCtl(motor == M1 ? IB1 : IB2, HIGH);
        } else {
            ioCtl(motor == M1 ? IA1 : IA2, HIGH);
            ioCtl(motor == M1 ? IB1 : IB2, LOW);
        }
    }

    public void stop(@IntRange(from = 1, to = 2) int motor) {
        if (motor == M1) {
            ioCtl(IA1, LOW);
            ioCtl(IB1, LOW);
        } else {
            ioCtl(IA2, LOW);
            ioCtl(IB2, LOW);
        }
    }

    public native int open();

    public native int close();

    /**
     * 控制io口高低电平
     *
     * @param pin 针脚索引
     *            IA1 : gpio pin 10
     *            IB1 : gpio pin 13
     *            IA2 : gpio pin 14
     *            IB2 : gpio pin 15
     * @param cmd 1高电平 0低电平
     * @return
     */
    public native int ioCtl(@IntRange(from = 0, to = 3) int pin, @IntRange(from = 0, to = 1) int cmd);

    static {
        System.loadLibrary("itop_l9110s");
    }
}
