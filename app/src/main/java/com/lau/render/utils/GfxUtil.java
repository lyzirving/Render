package com.lau.render.utils;

import android.graphics.PointF;

public class GfxUtil {
    static {
        System.loadLibrary("lib-render");
    }

    private static final String TAG = "GfxUtil";

    public static boolean hitAABB(PointF start, PointF dir, PointF AA, PointF BB) {
        if (start == null || dir == null || AA == null || BB == null) {
            LogUtil.d(TAG, "hitAABB: input is invalid");
            return false;
        }
        float[] s = new float[]{start.x, start.y};
        float[] d = new float[]{dir.x, dir.y};
        float[] aabb = new float[]{AA.x, AA.y, BB.x, BB.y};
        return nHitAABB(s, d, aabb);
    }

    private static native boolean nHitAABB(float[] start, float[] dir, float[] aabb);
}
