package com.lau.render;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;

import com.lau.render.ui.BVHActivity;
import com.lau.render.ui.RayTraceActivity;
import com.lau.render.ui.ShadowActivity;
import com.lau.render.utils.AssetsManager;
import com.lau.render.utils.LogUtil;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener,
                                                               AssetsManager.AssetsListener {
    private static final int CODE_REQUEST_READ_WRITE_PERMISSION = 0x01;
    private static final String TAG = "MainActivity";
    private MyHandler mHandler;

    private class MyHandler extends Handler {

        MyHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case 0: {
                    enableBtn(true);
                    break;
                }
                case 1: {
                    enableBtn(false);
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtil.d(TAG, "onCreate");
        setContentView(R.layout.activity_main);

        mHandler = new MyHandler(Looper.getMainLooper());
        AssetsManager.get().setListener(this);
        findViewById(R.id.btn_shadow).setOnClickListener(this);
        findViewById(R.id.btn_ray_trace).setOnClickListener(this);
        findViewById(R.id.btn_bvh).setOnClickListener(this);

        requestUserPermission(CODE_REQUEST_READ_WRITE_PERMISSION);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        LogUtil.d(TAG, "onRequestPermissionsResult: code = " + requestCode);
        switch (requestCode) {
            case CODE_REQUEST_READ_WRITE_PERMISSION: {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED && grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                    onPermissionGranted();
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        LogUtil.d(TAG, "onDestroy");
        AssetsManager.get().destroy();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_shadow: {
                startActivity(ShadowActivity.class);
                break;
            }
            case R.id.btn_bvh: {
                startActivity(BVHActivity.class);
                break;
            }
            case R.id.btn_ray_trace: {
                startActivity(RayTraceActivity.class);
                break;
            }
            default: {
                break;
            }
        }
    }

    private void requestUserPermission(int type) {
        switch (type) {
            case CODE_REQUEST_READ_WRITE_PERMISSION: {
                int permissionState = ActivityCompat.checkSelfPermission(getApplicationContext(), Manifest.permission.WRITE_EXTERNAL_STORAGE);
                if (permissionState != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(this,
                            new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE},
                            CODE_REQUEST_READ_WRITE_PERMISSION);
                } else {
                    onPermissionGranted();
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    private void onPermissionGranted() {
        LogUtil.d(TAG, "onPermissionGranted");
        AssetsManager.get().copyAssets(getApplicationContext());
    }

    @Override
    public void onCopyFinish() {
        mHandler.sendEmptyMessage(0);
    }

    @Override
    public void onCopyFail() {
        mHandler.sendEmptyMessage(1);
    }

    private void enableBtn(boolean enable) {
        findViewById(R.id.btn_shadow).setEnabled(enable);
        findViewById(R.id.btn_bvh).setEnabled(enable);
        findViewById(R.id.btn_ray_trace).setEnabled(enable);
    }

    private void startActivity(Class<?> activityClass) {
        Intent intent = new Intent();
        intent.setClass(getApplicationContext(), activityClass);
        startActivity(intent);
    }
}