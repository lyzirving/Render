package com.lau.render.scene;

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

    private static native void nAttachView(int id);
    private static native void nDetachView(int id);
}