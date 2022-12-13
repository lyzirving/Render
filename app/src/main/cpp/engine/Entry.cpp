/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   connection between android application and native layer
**/

#include <jni.h>

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Entry"

bool entry_point(JNIEnv *env)
{
    return true;
}

