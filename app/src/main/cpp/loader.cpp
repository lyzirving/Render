#include <jni.h>
#include <string>

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "loader"

extern bool entry_point(JNIEnv* env);

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOG_ERR("fail to get JNI environment");
        goto error;
    }

    if(!entry_point(env))
    {
        LOG_ERR("entry point failed");
        goto error;
    }

    LOG_DEBUG("succeed to load JNI environment");
    return JNI_VERSION_1_6;

    error:
    return JNI_ERR;
}