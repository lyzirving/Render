#ifndef RENDER_SYSTEMUTIL_H
#define RENDER_SYSTEMUTIL_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   support for function about system
**/

#include <chrono>

static uint64_t milliTime()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

static uint64_t microTime()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

#endif //RENDER_SYSTEMUTIL_H
