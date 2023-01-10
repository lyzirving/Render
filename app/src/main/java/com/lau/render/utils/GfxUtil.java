package com.lau.render.utils;

import android.graphics.PointF;

public class GfxUtil {
    static {
        System.loadLibrary("lib-render");
    }

    private static final String TAG = "GfxUtil";

    public static boolean hitAABB(PointF start, PointF end, PointF AA, PointF BB, int width, int height) {
        if (start == null || end == null || AA == null || BB == null) {
            LogUtil.d(TAG, "hitAABB: input is invalid");
            return false;
        }
        float[] s = new float[]{start.x, start.y};
        float[] e = new float[]{end.x, end.y};
        float[] aabb = new float[]{AA.x, AA.y, BB.x, BB.y};
        screen2Gfx(s, 0, width, height);
        screen2Gfx(e, 0, width, height);
        screen2Gfx(aabb, 0, width, height);
        screen2Gfx(aabb, 2, width, height);
        return nHitAABB(s, e, aabb);
    }

    public static void screen2Gfx(float[] pt, int start, float width, float height) {
        pt[start] = (pt[start] - width / 2.f) / (width / 2.f);
        pt[start + 1] = (height / 2.f - pt[start + 1]) / (height / 2.f);
    }

    private static native boolean nHitAABB(float[] start, float[] end, float[] aabb);
}
