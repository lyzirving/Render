package com.lau.render.ui;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import com.lau.render.R;
import com.lau.render.utils.LogUtil;

public class HitAABBActivity extends AppCompatActivity {

    private static final String TAG = "HitAABBActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtil.d(TAG, "onCreate");
        setContentView(R.layout.activity_hit_aabb);

    }
}
