package com.jiangyt.simple.itop4412.camera;

import android.view.SurfaceView;

/**
 * 类说明：摄像头基类
 * <p>
 * 包名： com.jiangyt.simple.itop4412.camera
 *
 * @author sinochem <a href="mailto:jiangyantaodev@163.com">jiangyt email</a>
 * @version 1.0
 * 创建日期：2021/2/20 下午1:28
 */
public interface ICamera {
    void setPreviewView(SurfaceView surfaceView);

    void onDestroy();

    void encodeStart(String outputPath);

    void encodeStop();
}
