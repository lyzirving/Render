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

    enum GreContextId : uint8_t
    {
        CTX_MAIN,
        CTX_SUB,
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
