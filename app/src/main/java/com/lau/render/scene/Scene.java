package com.lau.render.scene;

import android.view.Surface;

/**
 * @author lyzirving
 */
public class Scene {
    static {
        System.loadLibrary("lib-render");
    }

    private final int mId;

    public Scene(@Def.VIEW_ID int id) {
        mId = id;
        nAttachView(id);
    }

    public int getId() {
        return mId;
    }

    public void detachView()
    {
        nDetachView(mId);
    }

    public void testInterface()
    {
        nTestInterface(mId);
    }

    private static native void nAttachView(int id);
    private static native void nAttachSurface(int id, Surface surface);
    private static native void nDetachView(int id);
    private static native void nTestInterface(int id);
}
