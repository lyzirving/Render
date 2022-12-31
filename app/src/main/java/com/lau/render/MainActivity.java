package com.lau.render;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;

import com.lau.render.scene.Def;
import com.lau.render.scene.Scene;
import com.lau.render.utils.AssetsManager;
import com.lau.render.utils.LogUtil;

import java.util.Objects;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener,
                                                               SurfaceHolder.Callback,
                                                               AssetsManager.AssetsListener
{
    private static final int CODE_REQUEST_READ_WRITE_PERMISSION = 0x01;
    private static final String TAG = "MainActivity";
    private Scene mScene;
    private SurfaceView mSurfaceView;
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
                    mScene = new Scene(Def.MAIN);
                    mSurfaceView.setVisibility(View.VISIBLE);
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
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                             WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        mHandler = new MyHandler(Looper.getMainLooper());
        AssetsManager.get().setListener(this);

        mSurfaceView = findViewById(R.id.view_surface);
        mSurfaceView.getHolder().addCallback(this);

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
        if(!Objects.isNull(mScene)) {
            mScene.detachView();
            mScene = null;
        }
    }

    @Override
    public void onClick(View v) { }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        LogUtil.d(TAG, "surfaceCreated");
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        LogUtil.d(TAG, "surfaceChanged, width = " + width + ", height = " + height);
        if(!Objects.isNull(mScene)) {
            mScene.attachSurface(holder.getSurface());
        }
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        LogUtil.d(TAG, "surfaceDestroyed");
        if(!Objects.isNull(mScene)) {
            mScene.detachSurface();
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

    }
}