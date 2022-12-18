package com.lau.render.scene;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import androidx.annotation.IntDef;

/**
 * @author lyzirving
 */
public class Def {
    public static final int MAIN = 0;
    public static final int SUB = 1;

    @IntDef({MAIN, SUB})
    @Retention(RetentionPolicy.SOURCE)
    public @interface VIEW_ID {}
}
