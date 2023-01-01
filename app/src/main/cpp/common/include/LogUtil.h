#ifndef RENDER_LOGUTIL_H
#define RENDER_LOGUTIL_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   support for uniformed log output
**/
#include <string>
#include <android/log.h>
#include <glm/glm.hpp>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LogUtil"

#define LIB_TAG "lib-render"

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

#define LOG_FUNC_ENTER          __android_log_print(ANDROID_LOG_DEBUG, LIB_TAG,      \
                                                    "[%s][%s][%d]: enter",           \
                                                    LOCAL_TAG, __FUNCTION__, __LINE__)

#define LOG_FUNC_EXIT           __android_log_print(ANDROID_LOG_DEBUG, LIB_TAG,      \
                                                    "[%s][%s][%d]: exit",            \
                                                    LOCAL_TAG, __FUNCTION__, __LINE__)

#define LOG_MAT4(mat4)                            \
{                                                 \
    std::string msg("\n");                        \
    \
    msg.append(std::to_string(mat4[0].x));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[1].x));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[2].x));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[3].x));        \
    msg.append("\n");                             \
    \
    msg.append(std::to_string(mat4[0].y));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[1].y));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[2].y));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[3].y));        \
    msg.append("\n");                             \
                                                  \
    msg.append(std::to_string(mat4[0].z));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[1].z));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[2].z));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[3].z));        \
    msg.append("\n");                             \
                                                  \
    msg.append(std::to_string(mat4[0].w));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[1].w));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[2].w));        \
    msg.append(" ");                              \
    msg.append(std::to_string(mat4[3].w));        \
                                                  \
    __android_log_print(ANDROID_LOG_DEBUG, LIB_TAG,        \
                        "[%s][%s][%d]: %s",                \
                        LOCAL_TAG, __FUNCTION__, __LINE__, \
                        msg.c_str());                      \
}
#endif
