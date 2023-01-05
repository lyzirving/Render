#ifndef RENDER_GREDEF_H
#define RENDER_GREDEF_H

/**
 * @author  lyzirving
 * @date    2022-12-14
 * @brief   a file containing type or constant definition
**/

#include <cstdint>
#include <string>

namespace gre
{
    #define GRE_SUCCESS  0
    #define GRE_ERROR    1

    enum GrePriority : uint8_t
    {
        LOW,
        NORMAL,
        HIGH,
        TOP
    };

    enum GreEventType : uint8_t
    {
        RENDER,
        UI,
        INSTANT,
        EVT_TYPE_CNT
    };

    enum GreEventId : uint8_t
    {
        REFRESH,
        ATTACH_SURFACE,
        DETACH_SURFACE,
        NONE
    };

    enum GreContextId : uint8_t
    {
        CTX_MAIN,
        CTX_SUB,
        CTX_SUB_1,
        CTX_COUNT
    };

    #define CTX_ID_TO_STR(id)           \
    (                                   \
        {                               \
            std::string str;            \
            switch(id)                  \
            {                           \
                case(CTX_MAIN):         \
                    str = "main";       \
                    break;              \
                case(CTX_SUB):          \
                    str = "sub";        \
                    break;              \
                default:                \
                    str = "unknown";    \
                    break;              \
            }                           \
            (str.c_str());              \
        }                               \
    )
}

#endif //RENDER_GREDEF_H
