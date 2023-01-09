package com.lau.render.ui;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

public class HitAABBView extends View {

    public HitAABBView(Context context) {
        this(context, null);
    }

    public HitAABBView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public HitAABBView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }
}
