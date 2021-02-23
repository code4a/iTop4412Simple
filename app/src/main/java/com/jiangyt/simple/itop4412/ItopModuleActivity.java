package com.jiangyt.simple.itop4412;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.text.TextUtils;
import android.text.format.Time;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import com.jiangyt.library.libitop.ItopL9110s;
import com.jiangyt.library.libitop.ItopRelay;
import com.jiangyt.library.libitop.ItopRfid;
import com.jiangyt.library.libitop.ItopStepMotor;
import com.jiangyt.library.libitop.Operation;
import com.jiangyt.library.libitop.UvcCamera;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class ItopModuleActivity extends AppCompatActivity implements View.OnClickListener  {

    private static final String TAG = "UVC";
    ItopRelay itopRelay;
    ItopRfid itopRfid;
    ItopStepMotor stepMotor;
    ItopL9110s itopL9110s;

    String rxIdCode = "";

    EditText rfidEdit;
    EditText motorStep;
    EditText motorSpeed;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_itop_module);
        //initItopRelay();
//        initItopRfid();
//        initItopStepMotor();
        initItopUvc();
//        initItopL9110s();
    
    }

    private void initItopL9110s() {
        Operation.execRootCmd("chmod  777 /dev/l9110s_ctl");
        itopL9110s = new ItopL9110s();
        int open = itopL9110s.open();
        if (open < 0) {
            Log.e(TAG, "Open l9110s device failed");
            Toast.makeText(ItopModuleActivity.this, "Open l9110s device failed", Toast.LENGTH_LONG).show();
        }
        Button forward = findViewById(R.id.dc_motor_forward);
        Button stop = findViewById(R.id.dc_motor_stop);
        Button reverse = findViewById(R.id.dc_motor_reverse);
        forward.setOnClickListener(this);
        stop.setOnClickListener(this);
        reverse.setOnClickListener(this);
    }

    private int width = 320;
    private int height = 240;
    private int dwidth = 640;
    private int dheight = 480;
    private int numbuf = 4;
    private int devid = 4;
    private int index = 0;
    private int ret = 0;
    private byte[] mdata;
    private byte[] mout;
    private boolean m_stop = false;
    private Bitmap bitmap;
    private ByteBuffer Imagbuf;
    private Handler mHandler;
    private boolean en_video = false;
    private Time mtime;
    private boolean a_stop = false;

    private Button mcap;
    private Button mvideo;
    private ImageView mImag;

    private void initItopUvc() {
        mcap = (Button) findViewById(R.id.mcap);
        mvideo = (Button) findViewById(R.id.mvideo);
        mImag = (ImageView) findViewById(R.id.mimg);
        mdata = new byte[width * height * 2];
        mout = new byte[dwidth * dheight * 2];
        bitmap = Bitmap.createBitmap(dwidth, dheight, Bitmap.Config.RGB_565);
        Imagbuf = ByteBuffer.wrap(mout);
        mHandler = new Handler();
        mtime = new Time();

        //Operation.execRootCmd(String.format("chmod  777 /dev/video%d", devid));
        ret = UvcCamera.open(devid);
        if (ret < 0) {
            Toast.makeText(ItopModuleActivity.this, "Open device failed", Toast.LENGTH_LONG).show();
            //onDestroy();
        }
        ret = UvcCamera.init(width, height, numbuf);
        if (ret < 0) {
            Toast.makeText(ItopModuleActivity.this, "Init device failed", Toast.LENGTH_LONG).show();
            //onDestroy();
        }
        ret = UvcCamera.streamon();
        if (ret < 0) {
            Toast.makeText(ItopModuleActivity.this, "Stream on failed", Toast.LENGTH_LONG).show();
            //onDestroy();
        }

        mvideo.setOnClickListener(new VideoListener());
        mcap.setOnClickListener(new CaptureListener());

        new VideoThread().start();

    }

    private void initItopStepMotor() {
        Operation.execRootCmd("chmod  777 /dev/step_motor_driver");
        stepMotor = new ItopStepMotor();
        stepMotor.open();
        motorStep = findViewById(R.id.motor_step);
        motorSpeed = findViewById(R.id.motor_speed);
        Button forward = findViewById(R.id.motor_forward);
        Button reverse = findViewById(R.id.motor_reverse);
        forward.setOnClickListener(this);
        reverse.setOnClickListener(this);
    }

    private void initItopRfid() {
        itopRfid = new ItopRfid();
        itopRfid.open();
        rfidEdit = findViewById(R.id.rfid_edit);
        Button send = findViewById(R.id.rfid_send);
        Button recv = findViewById(R.id.rfid_recv);
        send.setOnClickListener(this);
        recv.setOnClickListener(this);
    }

    private void initItopRelay() {
        Operation.execRootCmd("chmod 777 /dev/relay_ctl");
        itopRelay = new ItopRelay();
        itopRelay.open();
        Button relayOpen = findViewById(R.id.relay_open);
        Button relayClose = findViewById(R.id.relay_close);
        relayOpen.setOnClickListener(this);
        relayClose.setOnClickListener(this);


        Button forward = findViewById(R.id.dc_motor_forward);
        Button reverse = findViewById(R.id.dc_motor_reverse);
        forward.setOnClickListener(this);
        reverse.setOnClickListener(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (itopRelay != null) {
            itopRelay.close();
            itopRelay = null;
        }
        if (itopRfid != null) {
            itopRfid.close();
            itopRfid = null;
        }
        if (stepMotor != null) {
            stepMotor.close();
            stepMotor = null;
        }
        if (itopL9110s != null) {
            itopL9110s.close();
            itopL9110s = null;
        }
        m_stop = true;
        en_video = false;
        UvcCamera.release();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.relay_open:
                itopRelay.ioCtl(1, 1);
                break;
            case R.id.relay_close:
                itopRelay.ioCtl(0, 0);
                break;
            case R.id.dc_motor_forward:
                itopL9110s.start(false, ItopL9110s.M1);
                break;
            case R.id.dc_motor_stop:
                itopL9110s.stop(ItopL9110s.M1);
                break;
            case R.id.dc_motor_reverse:
                itopL9110s.start(true, ItopL9110s.M1);
                break;
            case R.id.rfid_send:
                // 清空数据
                rfidEdit.setText("");
                break;
            case R.id.rfid_recv:
                rxIdCode = "";
                String str;
                int i;
                byte[] rx = itopRfid.readCardNum();
                if (rx != null) {
                    rfidEdit.setText(Operation.toHexString(rx, 0, rx.length));
                } else {
                    rfidEdit.setText("No Card Found!");
                }
                break;
            case R.id.motor_forward:
                stepMotorRun(false);
                break;
            case R.id.motor_reverse:
                stepMotorRun(true);
                break;
            default:
                break;
        }
    }

    void stepMotorRun(boolean reverse) {
        String stepF = motorStep.getText().toString();
        String speedF = motorSpeed.getText().toString();
        int stF = TextUtils.isEmpty(stepF) ? 0 : Integer.parseInt(stepF);
        int spF = TextUtils.isEmpty(speedF) ? 0 : Integer.parseInt(speedF);
        stepMotor.stepMotorNum(reverse, stF, spF);
    }


    class CaptureListener implements View.OnClickListener {

        @Override
        public void onClick(View v) {
            // TODO Auto-generated method stub
            //new CapThread().start();
            //Fimcgzsd.streamoff();
            saveMyBitmap(bitmap);
            //Fimcgzsd.streamon();
            Toast.makeText(ItopModuleActivity.this, "Capture Successfully", Toast.LENGTH_LONG).show();
        }

    }

    public static void saveMyBitmap(Bitmap mBitmap) {
        Time mtime = new Time();
        mtime.setToNow();
        File fdir = new File(Environment.getExternalStorageDirectory().getPath() + "/DCIM/" + "/gzsd/");
        if (!fdir.exists()) {
            fdir.mkdir();
        }
        File f = new File(Environment.getExternalStorageDirectory().getPath() + "/DCIM/" + "/gzsd/" + mtime.year + mtime.month + mtime.monthDay + mtime.hour + mtime.minute + mtime.second + ".png");
        try {
            f.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        FileOutputStream fOut = null;
        try {
            fOut = new FileOutputStream(f);
            mBitmap.compress(Bitmap.CompressFormat.PNG, 100, fOut);
            fOut.flush();
            fOut.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    class VideoListener implements View.OnClickListener {

        @Override
        public void onClick(View v) {
            // TODO Auto-generated method stub
            if (mvideo.getText().equals("Video")) {
                Log.d(TAG, "++++++++++++start");
                en_video = true;
                mtime.setToNow();
                //+ "/DCIM/" + "gzsd/"
                String videofile = Environment.getExternalStorageDirectory().getPath() + "/DCIM/" + mtime.year + mtime.month + mtime.monthDay + mtime.hour + mtime.minute + mtime.second + ".mpeg";
                ret = UvcCamera.videoinit(videofile.getBytes());
                if (ret < 0) {
                    Toast.makeText(ItopModuleActivity.this, "++video initfailed", Toast.LENGTH_LONG).show();
                    onDestroy();
                }
                mvideo.setText("停止");
                new SvideoThread().start();
            } else {
                Log.d(TAG, "++++++++++++stop");
                en_video = false;
                while (!a_stop) ;
                a_stop = false;
                UvcCamera.videoclose();
                mvideo.setText("视频");
            }
        }

    }

    class SvideoThread extends Thread {

        @Override
        public void run() {
            // TODO Auto-generated method stub
            //super.run();
            while (true) {
                if (!en_video) {
                    a_stop = true;
                    break;
                }
                UvcCamera.videostart(mdata);
            }
        }

    }

    final Runnable mUpdateUI = new Runnable() {

        @Override
        public void run() {
            // TODO Auto-generated method stub
            bitmap.copyPixelsFromBuffer(Imagbuf);
            mImag.setImageBitmap(bitmap);
            Imagbuf.clear();

        }
    };

    class VideoThread extends Thread {
        @Override
        public void run() {
            // TODO Auto-generated method stub
            //super.run();
            while (true) {
                if (m_stop) {
                    m_stop = false;
                    break;
                }
                index = UvcCamera.dqbuf(mdata);
                if (index < 0) {
                    //onDestroy();
                    break;
                }
                UvcCamera.yuvtorgb(mdata, mout, dwidth, dheight);
                mHandler.post(mUpdateUI);
                UvcCamera.qbuf(index);
            }
        }
    }


    class StartThread extends Thread {

        @Override
        public void run() {
            // TODO Auto-generated method stub
            //super.run();
            while (true) {
                index = UvcCamera.dqbuf(mdata);
                if (index < 0) {
                    onDestroy();
                    break;
                }
                mHandler.post(mUpdateUI);
                bitmap = BitmapFactory.decodeByteArray(mdata, 0, width * height);
                UvcCamera.qbuf(index);
            }
        }

    }
}