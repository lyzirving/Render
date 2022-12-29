#ifndef RENDER_GFXHELPER_H
#define RENDER_GFXHELPER_H

#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <android/log.h>

#include "LogUtil.h"

namespace GfxHelper
{
    uint32_t colorFormat(int component)
    {
        if (component == 1)
            return GL_RED;
        else if (component == 2)
            return GL_RG;
        else if(component == 3)
            return GL_RGB;
        else
            return GL_RGBA;
    }

    bool checkGlErr(const char *fmt, ...)
    {
        GLenum ret = glGetError();
        if (ret == GL_NO_ERROR)
        {
            return true;
        }
        else
        {
            char buf[512] = {0};
            va_list ap;
            va_start(ap, fmt);
            vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
            va_end(ap);
            __android_log_print(ANDROID_LOG_ERROR, LIB_TAG, "[%s]%s, err[0x%x]",
                                __FUNCTION__, buf, ret);
            return false;
        }
    }
}

#endif //RENDER_GFXHELPER_H
