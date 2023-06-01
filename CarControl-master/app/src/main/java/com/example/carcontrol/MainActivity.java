package com.example.carcontrol;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.RotateAnimation;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Delayed;

public class MainActivity extends AppCompatActivity {
    private BluetoothController mController = new BluetoothController();
    private List<BluetoothDevice> mDeviceList = new ArrayList<>();
    private List<BluetoothDevice> mBondedDeviceList = new ArrayList<>();
    private ListView mListView;
    private DeviceAdapter mAdapter;
    private Toast mToast;
    private Handler mUIHandler = new MyHandler();
    private AcceptThread mAcceptThread;
    private ConnectThread mConnectThread;
    private boolean ConnectFlag = false;
    private ImageButton imageButtonqianjin, imageButtonhoutui, imageButtonzuozhuan, imageButtonyouzhuan;
    private Button buttonsousuo, buttonchakan;
    private TextView text_tips, text_direction, text_radar;
    int viewflag = 0;


    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main1);
        initUI();
//        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED){
//            ActivityCompat.requestPermissions(MainActivity.this,new String[]{Manifest.permission.ACCESS_FINE_LOCATION},200);
//        }
        IntentFilter filter = new IntentFilter();
        //开始查找
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        //结束查找
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        //查找设备
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        //设备扫描状态改变
        filter.addAction(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED);
        //绑定状态
        filter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);

        registerReceiver(mReceiver, filter);
        mController.turnOnBluetooth(this, 0);
        ////////////////////////////////////////////////////////////
        mBondedDeviceList = mController.getBondedDeviceList();
        mAdapter.refresh(mBondedDeviceList);
        mListView.setOnItemClickListener(bondedDeviceClick);
        imageButtonqianjin.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        say("1");
                        text_direction.setText("正在直行");
                        //showToast("加油门");
                        //textViewmodle.setText("前进");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_qianjin);
                        break;
                    case MotionEvent.ACTION_UP:
                        say("O");
                        text_direction.setText("停车ing");
                        //showToast("松油门");
                        //textViewmodle.setText("停车");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_tingche);
                        break;
                }
                return true;
            }
        });
        imageButtonhoutui.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        say("2");
                        text_direction.setText("正在倒车");
                        //showToast("停车");
                        //textViewmodle.setText("后退");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_houtui);
                        break;
                    case MotionEvent.ACTION_UP:
                        say("O");
                        text_direction.setText("停车ing");
                        //showToast("松刹车");
                        //textViewmodle.setText("停车");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_tingche);
                        break;
                }
                return true;
            }
        });
        imageButtonzuozhuan.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        say("4");
                        text_direction.setText("正在左转");
                        //showToast("左转");
                        //textViewmodle.setText("左转");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_zuozhuan);
                        break;
                    case MotionEvent.ACTION_UP:
                        say("O");
                        text_direction.setText("停车ing");
                        //showToast("回正");
                        //textViewmodle.setText("停车");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_tingche);
                        break;
                }
                return true;
            }
        });
        imageButtonyouzhuan.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        say("3");
                        text_direction.setText("正在右转");
                        //showToast("右转");
                        //textViewmodle.setText("右转");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_youzhuan);
                        break;
                    case MotionEvent.ACTION_UP:
                        say("O");
                        text_direction.setText("停车ing");
                        //showToast("回正");
                        //textViewmodle.setText("停车");
                        //imageViewxiaoche.setImageResource(R.drawable.ic_tingche);
                        break;
                }
                return true;
            }
        });
    }


    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)) {
                setProgressBarIndeterminateVisibility(true);
                //初始化数据列表
                mDeviceList.clear();
                mAdapter.notifyDataSetChanged();
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                setProgressBarIndeterminateVisibility(false);
            } else if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                //找到一个添加一个
                mDeviceList.add(device);
                mAdapter.notifyDataSetChanged();
            } else if (BluetoothAdapter.ACTION_SCAN_MODE_CHANGED.equals(action)) {
                int scanMode = intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE, 0);
                if (scanMode == BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
                    setProgressBarIndeterminateVisibility(true);
                } else {
                    setProgressBarIndeterminateVisibility(false);
                }
            } else if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(action)) {
                BluetoothDevice remoteDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (remoteDevice == null) {
                    showToast("没有找到设备");
                    return;
                }
                int status = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, 0);
                if (status == BluetoothDevice.BOND_BONDED) {
                    showToast("设备已绑定" + remoteDevice.getName());
                } else if (status == BluetoothDevice.BOND_BONDING) {
                    showToast("绑定中..." + remoteDevice.getName());
                } else if (status == BluetoothDevice.BOND_NONE) {
                    showToast("绑定失败" + remoteDevice.getName());
                }
            }
        }
    };

    //未绑定设备点击事件
    private AdapterView.OnItemClickListener bindDeviceClick = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            BluetoothDevice device = mDeviceList.get(position);
            device.createBond();
        }
    };

    //已绑定设备点击事件
    private AdapterView.OnItemClickListener bondedDeviceClick = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            BluetoothDevice device = mBondedDeviceList.get(position);
            if (mConnectThread != null) {
                mConnectThread.cancel();
            }
            mConnectThread = new ConnectThread(device, mController.getAdapter(), mUIHandler);
            mConnectThread.start();
        }
    };

    private void initUI() {
        mListView = (ListView) findViewById(R.id.device_list);
        mAdapter = new DeviceAdapter(mDeviceList, this);
        mListView.setAdapter(mAdapter);
        mListView.setOnItemClickListener(bindDeviceClick);
        buttonsousuo = findViewById(R.id.buttonsousuo);
        buttonchakan = findViewById(R.id.buttonchakan);
        imageButtonqianjin = findViewById(R.id.imageButtonqianjin);
        imageButtonhoutui = findViewById(R.id.imageButtonhoutui);
        imageButtonzuozhuan = findViewById(R.id.imageButtonzuozhuan);
        imageButtonyouzhuan = findViewById(R.id.imageButtonyouzhuan);
        text_tips = findViewById(R.id.text_tips);
        text_direction = findViewById(R.id.text_direction);
        text_radar = findViewById(R.id.text_radar);


    }

    private class MyHandler extends Handler {
        public void handleMessage(Message message) {
            super.handleMessage(message);
            switch (message.what) {
                case Constant.MSG_GOT_DATA:
                    Log.i("message", message.obj.toString());
                    if (message.obj.toString().equals("a") || message.obj.toString().equals("aa") || message.obj.toString().equals("aaa")) {
                        text_radar.setText("有障碍，请停车！");
                    } else {
                        text_radar.setText("前方无障碍");
                    }

                    break;
                case Constant.MSG_ERROR:
                    showToast("error:" + String.valueOf(message.obj));
                    text_tips.setText("车辆无法连接");
                    break;
                case Constant.MSG_CONNECTED_TO_SERVER:
                    showToast("连接到服务端");
                    mListView.setVisibility(View.GONE);
                    imageButtonqianjin.setVisibility(View.VISIBLE);
                    imageButtonzuozhuan.setVisibility(View.VISIBLE);
                    imageButtonyouzhuan.setVisibility(View.VISIBLE);
                    imageButtonhoutui.setVisibility(View.VISIBLE);
                    text_tips.setText("626已连接");
                    break;
                case Constant.MSG_GOT_A_CLINET:
                    showToast("找到服务端");
                    text_tips.setText("找到服务端");
                    break;
            }
        }
    }

    private void showToast(String text) {
        if (mToast == null) {
            mToast = Toast.makeText(this, text, Toast.LENGTH_SHORT);
        } else {
            mToast.setText(text);
        }
        mToast.show();
    }

    //搜索设备
    public void SearchDevice(View view) {

        mListView.setVisibility(View.VISIBLE);
        viewflag = 1;
        showToast("搜索中...");
        mAdapter.refresh(mDeviceList);
        mController.findDevice();
        mListView.setOnItemClickListener(bindDeviceClick);

    }

    //查看已绑定设备
    public void ShowBondDevice(View view) {
        mListView.setVisibility(View.VISIBLE);
        viewflag = 2;
        mBondedDeviceList = mController.getBondedDeviceList();
        mAdapter.refresh(mBondedDeviceList);
        mListView.setOnItemClickListener(bondedDeviceClick);

    }

    private void say(String word) {
        if (mAcceptThread != null) {
            try {
                mAcceptThread.sendData(word.getBytes("utf-8"));
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        } else if (mConnectThread != null) {
            try {
                mConnectThread.sendData(word.getBytes("utf-8"));
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
    }


}
