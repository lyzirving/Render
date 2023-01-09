package com.lau.render.utils;

import android.graphics.PointF;

public class GfxUtil {
    static {
        System.loadLibrary("lib-render");
    }

    public static boolean hitAABB(PointF start, PointF dir, PointF AA, PointF BB) {
        return nHitAABB(null, null, null);
    }

    private static native boolean nHitAABB(float[] start, float[] dir, float[] aabb);
}
