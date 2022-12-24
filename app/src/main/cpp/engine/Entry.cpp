/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   connection between android application and native layer
**/
#include <jni.h>
#include <android/native_window_jni.h>

#include "GreProxy.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Entry"

#define CLASS_SCENE "com/lau/render/scene/Scene"

static void nativeAttachView(JNIEnv *env, jclass clazz, jint id) {
    gre::PROXY_attachView(id);
}

static void nativeDetachView(JNIEnv *env, jclass clazz, jint id) {
    gre::PROXY_detachView(id);
}

static void nativeAttachSurface(JNIEnv *env, jclass clazz, jint id, jobject surface) {
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if(!gre::PROXY_attachSurface(id, window))
    {
        ANativeWindow_release(window);
    }
}

static void nativeTestInterface(JNIEnv *env, jclass clazz, jint id) {
    gre::PROXY_testInterface(id);
}

static JNINativeMethod methods[] = {
        {
                "nAttachView",
                "(I)V",
                (void *) nativeAttachView
        },
        {
                "nDetachView",
                "(I)V",
                (void *) nativeDetachView
        },
        {
                "nAttachSurface",
                "(ILandroid/view/Surface;)V",
                (void *) nativeAttachSurface
        },
        {
                "nTestInterface",
                "(I)V",
                (void *) nativeTestInterface
        },
};

bool entry_point(JNIEnv *env)
{
    jclass clazz{nullptr};
    int cnt = sizeof(methods) / sizeof(methods[0]);

    clazz = env->FindClass(CLASS_SCENE);
    if(!clazz || env->RegisterNatives(clazz, methods, cnt) < 0)
    {
        LOG_ERR("fail to load class[%s]", CLASS_SCENE);
        goto err;
    }

    return true;

err:
    return false;
}

