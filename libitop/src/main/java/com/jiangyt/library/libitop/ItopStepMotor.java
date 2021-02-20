package com.jiangyt.library.libitop;

/**
 * Desc: 步进电机
 * <p>
 *
 * @author Create by sinochem on 2020/10/10
 * <p>
 * Version: 1.0.0
 */
public class ItopStepMotor {

    private static int CMD_STEPMOTOR_A = 0;
    private static int CMD_STEPMOTOR_B = 1;
    private static int CMD_STEPMOTOR_C = 2;
    private static int CMD_STEPMOTOR_D = 3;

    private static int HIGH = 1;
    private static int LOW = 0;

    public void stepMotorNum(boolean reverse, int num, int speed) {
        int step = 0;
        for (int i = 0; i < num; i++) {
            if (!reverse) {
                step++;
                if (step > 7) step = 0;
            } else {
                if (step == 0) step = 8;
                step--;
            }
            stepTurn(step, speed);
        }
    }

    private void stepTurn(int step, int delayTime) {
        switch (step) {
            case 0:
                ioCtl(HIGH, CMD_STEPMOTOR_A);
                ioCtl(LOW, CMD_STEPMOTOR_B);
                ioCtl(LOW, CMD_STEPMOTOR_C);
                ioCtl(LOW, CMD_STEPMOTOR_D);
                break;
            case 1:
                ioCtl(HIGH, CMD_STEPMOTOR_A);
                ioCtl(HIGH, CMD_STEPMOTOR_B);
                ioCtl(LOW, CMD_STEPMOTOR_C);
                ioCtl(LOW, CMD_STEPMOTOR_D);
                break;
            case 2:
                ioCtl(LOW, CMD_STEPMOTOR_A);
                ioCtl(HIGH, CMD_STEPMOTOR_B);
                ioCtl(LOW, CMD_STEPMOTOR_C);
                ioCtl(LOW, CMD_STEPMOTOR_D);
                break;
            case 3:
                ioCtl(LOW, CMD_STEPMOTOR_A);
                ioCtl(HIGH, CMD_STEPMOTOR_B);
                ioCtl(HIGH, CMD_STEPMOTOR_C);
                ioCtl(LOW, CMD_STEPMOTOR_D);
                break;
            case 4:
                ioCtl(LOW, CMD_STEPMOTOR_A);
                ioCtl(LOW, CMD_STEPMOTOR_B);
                ioCtl(HIGH, CMD_STEPMOTOR_C);
                ioCtl(LOW, CMD_STEPMOTOR_D);
                break;
            case 5:
                ioCtl(LOW, CMD_STEPMOTOR_A);
                ioCtl(LOW, CMD_STEPMOTOR_B);
                ioCtl(HIGH, CMD_STEPMOTOR_C);
                ioCtl(HIGH, CMD_STEPMOTOR_D);
                break;
            case 6:
                ioCtl(LOW, CMD_STEPMOTOR_A);
                ioCtl(LOW, CMD_STEPMOTOR_B);
                ioCtl(LOW, CMD_STEPMOTOR_C);
                ioCtl(HIGH, CMD_STEPMOTOR_D);
                break;
            case 7:
                ioCtl(HIGH, CMD_STEPMOTOR_A);
                ioCtl(LOW, CMD_STEPMOTOR_B);
                ioCtl(LOW, CMD_STEPMOTOR_C);
                ioCtl(HIGH, CMD_STEPMOTOR_D);
                break;
            default:
                break;
        }
        try {
            Thread.currentThread();
            Thread.sleep(delayTime);
        } catch (InterruptedException e) {
            e.printStackTrace();
            ioCtl(LOW, CMD_STEPMOTOR_A);
            ioCtl(LOW, CMD_STEPMOTOR_B);
            ioCtl(LOW, CMD_STEPMOTOR_C);
            ioCtl(LOW, CMD_STEPMOTOR_D);
        }
        try {
            Thread.currentThread();
            Thread.sleep(delayTime);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public native int open();

    public native int close();

    public native int ioCtl(int num, int en);

    static {
        System.loadLibrary("step_motor");
    }
}
