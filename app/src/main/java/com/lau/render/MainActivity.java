package com.lau.render;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.lau.render.scene.Def;
import com.lau.render.scene.Scene;
import com.lau.render.utils.LogUtil;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener,
                                                               SurfaceHolder.Callback
{
    private static final String TAG = "MainActivity";
    private Scene mScene;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtil.d(TAG, "onCreate");
        setContentView(R.layout.activity_main);
        mScene = new Scene(Def.MAIN);

        SurfaceView surfaceView = findViewById(R.id.view_surface);
        surfaceView.getHolder().addCallback(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        LogUtil.d(TAG, "onDestroy");
        mScene.detachView();
        mScene = null;
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
        mScene.attachSurface(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        LogUtil.d(TAG, "surfaceDestroyed");
        mScene.detachSurface();
    }
}