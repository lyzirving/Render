/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   connection between android application and native layer
**/
#include <jni.h>
#include <android/native_window_jni.h>

#include "GfxLib.h"
#include "GreProxy.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Entry"

#define CLASS_SCENE "com/lau/render/scene/Scene"
#define CLASS_GFX_UTIL "com/lau/render/utils/GfxUtil"

static void nativeAttachView(JNIEnv *env, jclass clazz, jint id)
{
    gre::PROXY_attachView(id);
}

static void nativeDetachView(JNIEnv *env, jclass clazz, jint id)
{
    gre::PROXY_detachView(id);
}

static void nativeAttachSurface(JNIEnv *env, jclass clazz, jint id, jobject surface)
{
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if(!gre::PROXY_attachSurface(id, window))
    {
        ANativeWindow_release(window);
    }
}

static void nativeDetachSurface(JNIEnv *env, jclass clazz, jint id)
{
    gre::PROXY_detachSurface(id);
}

static void nativeTestInterface(JNIEnv *env, jclass clazz, jint id)
{
    gre::PROXY_testInterface(id);
}

static jboolean nativeHitAABB(JNIEnv *env, jclass clazz, jfloatArray jStart, jfloatArray jDir, jfloatArray jAABB)
{
    if(env->GetArrayLength(jStart) != 2 || !jStart)
    {
        LOG_ERR("start float array's length is not 2 or ptr is null[%s]", !jStart ? "true" : "false");
        assert(0);
    }
    if(env->GetArrayLength(jDir) != 2 || !jDir)
    {
        LOG_ERR("dir float array's length is not 2 or ptr is null[%s]", !jDir ? "true" : "false");
        assert(0);
    }
    if(env->GetArrayLength(jAABB) != 4 || !jAABB)
    {
        LOG_ERR("aabb float array's length is not 4 or ptr is null[%s]", !jAABB ? "true" : "false");
        assert(0);
    }
    jfloat *start = env->GetFloatArrayElements(jStart, nullptr);
    jfloat *dir = env->GetFloatArrayElements(jDir, nullptr);
    jfloat *aabb = env->GetFloatArrayElements(jAABB, nullptr);

    glm::vec2 s{start[0], start[1]};
    glm::vec2 d{dir[0], dir[1]};
    glm::vec2 aa{aabb[0], aabb[1]};
    glm::vec2 bb{aabb[2], aabb[3]};

    float dist = gfx::gfx2d::hitAABB(s, d, aa, bb);

    return dist > 0.f;
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
                "nDetachSurface",
                "(I)V",
                (void *) nativeDetachSurface
        },
        {
                "nTestInterface",
                "(I)V",
                (void *) nativeTestInterface
        },
};

static JNINativeMethod utilMethods[] = {
        {
                "nHitAABB",
                "([F[F[F)Z",
                (void *) nativeHitAABB
        },
};

bool entry_point(JNIEnv *env)
{
    jclass clazz{nullptr}, utilClazz{nullptr};
    int cnt = sizeof(methods) / sizeof(methods[0]);
    int utilMethodCnt = sizeof(utilMethods) / sizeof(utilMethods[0]);

    clazz = env->FindClass(CLASS_SCENE);
    if(!clazz || env->RegisterNatives(clazz, methods, cnt) < 0)
    {
        LOG_ERR("fail to load class[%s]", CLASS_SCENE);
        goto err;
    }

    utilClazz = env->FindClass(CLASS_GFX_UTIL);
    if(!utilClazz || env->RegisterNatives(utilClazz, utilMethods, utilMethodCnt) < 0)
    {
        LOG_ERR("fail to load class[%s]", utilClazz);
        goto err;
    }

    return true;

err:
    return false;
}

