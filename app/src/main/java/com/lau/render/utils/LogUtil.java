package com.lau.render.utils;

import android.util.Log;

/**
 * @author lyzirving
 */
public class LogUtil {
    private static final String TAG = "lib-render";

    public static void d(String tag, String msg) {
        Log.d(TAG, tag + "__" + msg);
    }

    public static void i(String tag, String msg) {
        Log.i(TAG, tag + "__" + msg);
    }

    public static void w(String tag, String msg) {
        Log.w(TAG, tag + "__" + msg);
    }

    public static void e(String tag, String msg) {
        Log.e(TAG, tag + "__" + msg);
    }
}
