#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <cstdio>

#include "GfxHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxHelper"

namespace gfx
{
    uint32_t GfxHelper::colorFormat(int component)
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

    bool GfxHelper::checkGlErr(const char *fmt, ...)
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
            std::vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
            va_end(ap);
            LOG_ERR("%s, err[0x%x]", buf, ret);
            return false;
        }
    }
}

