#ifndef RENDER_LOGUTIL_H
#define RENDER_LOGUTIL_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   support for uniformed log output
**/

#include <android/log.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LogUtil"

#define LIB_TAG "Render"

#define LOG_DEBUG(format, ...)  __android_log_print(ANDROID_LOG_DEBUG,               \
                                                    LIB_TAG, "[%s][%s][%d] " format, \
                                                    LOCAL_TAG, __FUNCTION__,         \
                                                    __LINE__, ##__VA_ARGS__)

#define LOG_INFO(format, ...)   __android_log_print(ANDROID_LOG_INFO,                \
                                                    LIB_TAG, "[%s][%s][%d] " format, \
                                                    LOCAL_TAG, __FUNCTION__,         \
                                                    __LINE__, ##__VA_ARGS__)

#define LOG_WARN(format, ...)   __android_log_print(ANDROID_LOG_WARN,                \
                                                    LIB_TAG, "[%s][%s][%d] " format, \
                                                    LOCAL_TAG, __FUNCTION__,         \
                                                    __LINE__, ##__VA_ARGS__)

#define LOG_ERR(format, ...)    __android_log_print(ANDROID_LOG_ERROR,               \
                                                    LIB_TAG, "[%s][%s][%d] " format, \
                                                    LOCAL_TAG, __FUNCTION__,         \
                                                    __LINE__, ##__VA_ARGS__)
#endif
