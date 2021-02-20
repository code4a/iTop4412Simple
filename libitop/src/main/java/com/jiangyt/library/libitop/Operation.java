package com.jiangyt.library.libitop;

import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Desc: com.jiangyt.simple.itop_relay
 * <p>
 *
 * @author Create by sinochem on 2020/10/10
 * <p>
 * Version: 1.0.0
 */
public class Operation {
    private static final String TAG = Operation.class.getSimpleName();
    private static final char[] HEX = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    public static final String COMMAND_SU = "su";
    public static final String COMMAND_SH = "sh";
    public static final String COMMAND_EXIT = "exit\n";
    public static final String COMMAND_LINE_END = "\n";

    private static boolean hasRoot = false;

    /**
     * 执行一条命令，是否需要root
     *
     * @param command 命令
     * @param isRoot  是否需要root
     * @return 执行结果
     */
    public static CommandResult execCommand(String command, boolean isRoot) {
        String[] commands = {command};
        return execCommand(commands, isRoot);
    }

    /**
     * 执行多条条命令，是否需要root
     *
     * @param commands 多条命令
     * @param isRoot   是否需要root
     * @return 执行结果
     */
    public static CommandResult execCommand(String[] commands, boolean isRoot) {
        CommandResult result = new CommandResult();
        if (commands == null || commands.length == 0) return result;
        Process process = null;
        DataOutputStream dos = null;
        BufferedReader successResult = null;
        BufferedReader errorResult = null;
        StringBuilder successMsg = null;
        StringBuilder errorMsg = null;

        try {
            process = Runtime.getRuntime().exec(isRoot ? COMMAND_SU : COMMAND_SH);
            dos = new DataOutputStream(process.getOutputStream());
            for (String command : commands) {
                if (command != null) {
                    dos.write(command.getBytes());
                    dos.writeBytes(COMMAND_LINE_END);
                    dos.flush();
                }
            }
            dos.writeBytes(COMMAND_EXIT);
            dos.flush();
            result.result = process.waitFor();
            // 获取错误信息
            successMsg = new StringBuilder();
            errorMsg = new StringBuilder();
            successResult = new BufferedReader(new InputStreamReader(process.getInputStream()));
            errorResult = new BufferedReader(new InputStreamReader(process.getErrorStream()));
            String s;
            while ((s = successResult.readLine()) != null) {
                successMsg.append(s);
            }
            while ((s = errorResult.readLine()) != null) {
                errorMsg.append(s);
            }
            result.successMsg = successMsg.toString();
            result.errorMsg = errorMsg.toString();
            Log.i(TAG, result.toString());
        } catch (IOException e) {
            String eMessage = e.getMessage();
            if (!TextUtils.isEmpty(eMessage)) {
                Log.e(TAG, eMessage);
            } else {
                e.printStackTrace();
            }
        } catch (Exception e) {
            String eMessage = e.getMessage();
            if (!TextUtils.isEmpty(eMessage)) {
                Log.e(TAG, eMessage);
            } else {
                e.printStackTrace();
            }
        } finally {
            try {
                if (dos != null) {
                    dos.close();
                }
                if (successResult != null) {
                    successResult.close();
                }
                if (errorResult != null) {
                    errorResult.close();
                }
            } catch (IOException e) {
                String eMessage = e.getMessage();
                if (!TextUtils.isEmpty(eMessage)) {
                    Log.e(TAG, eMessage);
                } else {
                    e.printStackTrace();
                }
            }
            if (process != null) process.destroy();
        }
        return result;
    }


    /**
     * 执行命令不关注结果
     *
     * @param cmd 命令
     * @return 是否成功的返回值
     */
    public static int execRootCmdSilent(String cmd) {
        int result = -1;
        DataOutputStream dos = null;

        try {
            // 经过Root处理的Android系统即有su命令
            Process p = Runtime.getRuntime().exec(COMMAND_SU);
            dos = new DataOutputStream(p.getOutputStream());
            Log.i(TAG, String.format("执行命令%s", cmd));
            dos.writeBytes(String.format("%s\n", cmd));
            dos.flush();
            dos.writeBytes(COMMAND_EXIT);
            dos.flush();
            p.waitFor();
            result = p.exitValue();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (dos != null) {
                try {
                    dos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    /**
     * 执行命令并输出结果
     *
     * @param cmd 命令
     * @return 结果
     */
    public static String execRootCmd(String cmd) {
        StringBuilder result = new StringBuilder();
        DataOutputStream dos = null;
        DataInputStream dis = null;

        try {
            // 经过Root处理的Android系统即有su命令
            Process p = Runtime.getRuntime().exec(COMMAND_SU);
            dos = new DataOutputStream(p.getOutputStream());
            dis = new DataInputStream(p.getInputStream());
            Log.i(TAG, String.format("执行命令%s", cmd));
            dos.writeBytes(String.format("%s\n", cmd));
            dos.flush();
            dos.writeBytes(COMMAND_EXIT);
            dos.flush();
            String line = null;
            while ((line = dis.readLine()) != null) {
                Log.d(TAG, line);
                result.append(line);
            }
            p.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (dos != null) {
                try {
                    dos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (dis != null) {
                try {
                    dis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result.toString();
    }

    public static boolean hasRoot() {
        if (!hasRoot) {
            // 通过执行测试命令来检测
            int ret = execRootCmdSilent("echo test");
            if (ret != -1) {
                Log.i(TAG, "拥有root权限");
                hasRoot = true;
            } else {
                Log.i(TAG, "暂未获取到root权限");
            }
        } else {
            Log.i(TAG, "已经拥有root权限");
        }
        return hasRoot;
    }

    /**
     * 运行多条命令
     *
     * @param cmd     命令数组
     * @param workDir 工作文件夹
     * @return 结果
     * @throws IOException io异常
     */
    public static synchronized String run(String[] cmd, String workDir) throws IOException {
        StringBuilder result = new StringBuilder();
        InputStream is = null;
        try {
            ProcessBuilder builder = new ProcessBuilder(cmd);
            // 设置一个路径（绝对路径了就不一定需要）
            if (workDir != null) {
                // 设置工作目录(同上)
                builder.directory(new File(workDir));
                // 合并标准错误和标准输出
                builder.redirectErrorStream(true);
                // 启动一个新进程
                Process process = builder.start();
                // 读取进程标准输出流
                is = process.getInputStream();
                byte[] re = new byte[1024];
                while (is.read(re) != -1) {
                    result.append(new String(re));
                }
            }
        } finally {
            if (is != null) {
                is.close();
            }
        }
        return result.toString();
    }

    /**
     * 获取wifi名的数组
     *
     * @param wifiInfo wifi信息
     * @return 数组
     */
    public static String[] getESSID(String wifiInfo) {
        int begin = wifiInfo.indexOf("ESSID:");
        int end = wifiInfo.indexOf("Mode:");
        String str[] = new String[20];
        // str数组第0位，存放一个数字，表示有多少个wifi
        int i = 1;
        while (begin != -1 && (end - begin) > 8) {
            String tstr = wifiInfo.substring(begin + 7, end);
            str[i] = tstr.substring(0, tstr.indexOf("\""));
            wifiInfo = wifiInfo.substring(end + 5);
            begin = wifiInfo.indexOf("ESSID:");
            end = wifiInfo.indexOf("Mode:");
            ++i;
        }
        str[0] = (i - 1) + "";
        return str;
    }

    /**
     * 获取wifi名数组
     *
     * @return 数组
     */
    public static String[] getWifiName() {
        hasRoot();
        execRootCmdSilent("insmod /data/mt7601Usta.ko");
        execRootCmdSilent("netcfg wlan0 up");
        execRootCmdSilent("netcfg eth0 down");
        String[] wifi = getESSID(execRootCmd("iwlist wlan0 scan"));
        int wifiCount = Integer.parseInt(wifi[0]);
        String[] ssid = new String[wifiCount];
        for (int i = 0; i < wifiCount; i++) {
            ssid[i] = wifi[i + 1];
        }
        return ssid;
    }

    /**
     * 线程休眠
     *
     * @param ms 休眠时长
     */
    public static void delay(int ms) {
        try {
            Thread.currentThread();
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static String toHexString(byte[] d, int s, int n) {
        final char[] ret = new char[n * 2];
        final int e = s + n;
        int x = 0;
        for (int i = s; i < e; ++i) {
            final byte v = d[i];
            ret[x++] = HEX[0x0F & (v >> 4)];
            ret[x++] = HEX[0x0F & (v)];
        }
        return new String(ret);
    }
}
