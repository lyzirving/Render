package com.lau.render.ui;

import android.os.Bundle;
import android.widget.RadioGroup;

import androidx.appcompat.app.AppCompatActivity;

import com.lau.render.R;
import com.lau.render.utils.LogUtil;

public class HitAABBActivity extends AppCompatActivity implements RadioGroup.OnCheckedChangeListener {

    private static final String TAG = "HitAABBActivity";
    private HitAABBView mHitAABBView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtil.d(TAG, "onCreate");
        setContentView(R.layout.activity_hit_aabb);
        ((RadioGroup)findViewById(R.id.radio_group)).setOnCheckedChangeListener(this);
        mHitAABBView = findViewById(R.id.view_hit_aabb);
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        switch (checkedId) {
            case R.id.radio_clear: {
                mHitAABBView.setMode(HitAABBView.ACT_CLEAR);
                break;
            }
            case R.id.radio_set_bound: {
                mHitAABBView.setMode(HitAABBView.ACT_BOUND);
                break;
            }
            case R.id.radio_set_anchor: {
                mHitAABBView.setMode(HitAABBView.ACT_RAY_ANCHOR);
                break;
            }
            case R.id.radio_set_end: {
                mHitAABBView.setMode(HitAABBView.ACT_RAY_END);
                break;
            }
            default: {
                break;
            }
        }
    }
}
