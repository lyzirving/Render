package com.lau.render.ui;

import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.lau.render.R;
import com.lau.render.scene.Def;
import com.lau.render.scene.Scene;
import com.lau.render.utils.LogUtil;

import java.util.Objects;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

/**
 * @author lyzirving
 */
public class BVHActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private static final String TAG = "RayTraceActivity";

    private Scene mScene;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtil.d(TAG, "onCreate");
        setContentView(R.layout.activity_bvh);

        SurfaceView surfaceView = findViewById(R.id.view_surface);
        surfaceView.getHolder().addCallback(this);

        mScene = new Scene(Def.SUB_1);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        LogUtil.d(TAG, "onDestroy");
        if(!Objects.isNull(mScene)) {
            mScene.detachView();
            mScene = null;
        }
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {}

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
}
