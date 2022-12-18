package com.lau.render;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.lau.render.scene.Def;
import com.lau.render.scene.Scene;
import com.lau.render.utils.LogUtil;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private Scene mScene;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtil.d(TAG, "onCreate");
        setContentView(R.layout.activity_main);
        mScene = new Scene(Def.MAIN);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        LogUtil.d(TAG, "onDestroy");
        mScene.detachView();
        mScene = null;
    }
}