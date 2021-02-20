package com.jiangyt.library.libitop;

/**
 * Desc: com.jiangyt.simple.itop_relay
 * <p>
 *
 * @author Create by sinochem on 2020/10/10
 * <p>
 * Version: 1.0.0
 */
public class CommandResult {

    public int result = -1;
    public String errorMsg;
    public String successMsg;

    @Override
    public String toString() {
        return "CommandResult{" +
                "result=" + result +
                ", errorMsg='" + errorMsg + '\'' +
                ", successMsg='" + successMsg + '\'' +
                '}';
    }
}
