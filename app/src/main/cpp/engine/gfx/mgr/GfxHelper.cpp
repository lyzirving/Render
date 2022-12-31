#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <cstdio>
#include <string>

#include "GfxHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxHelper"

namespace gfx
{
    uint32_t GfxHelper::createProgram(const char *vert, const char *frag, const char *geo)
    {
        GLuint vertShader{0};
        GLuint fragShader{0};
        GLuint geoShader{0};
        GLuint program{0};
        GLint linkStatus{GL_FALSE};

        bool compileGeo = (geo != nullptr) && (std::strlen(geo) != 0);
        if (vert == nullptr || std::strlen(vert) == 0)
        {
            LOG_ERR("input vertex shader is null");
            goto err;
        }
        if (frag == nullptr || std::strlen(frag) == 0)
        {
            LOG_ERR("input fragment shader is null");
            goto err;
        }
        vertShader = createShader(GL_VERTEX_SHADER, vert);
        if(!vertShader)
        {
            LOG_ERR("fail to compile vertex shader %s", vert);
            goto err;
        }
        fragShader = createShader(GL_FRAGMENT_SHADER, frag);
        if(!fragShader)
        {
            LOG_ERR("fail to compile fragment shader %s", frag);
            goto err;
        }
        if(compileGeo)
        {
            geoShader = createShader(GL_GEOMETRY_SHADER, geo);
            if(!geoShader)
            {
                LOG_ERR("fail to compile geometry shader %s", geo);
                goto err;
            }
        }

        program = glCreateProgram();
        if(!program)
        {
            checkGlErr("fail to create program");
            goto err;
        }
        glAttachShader(program, vertShader);
        if(!checkGlErr("fail to attach vertex shader"))
            goto err;

        glAttachShader(program, fragShader);
        if(!checkGlErr("fail to attach fragment shader"))
            goto err;

        if(compileGeo)
        {
            glAttachShader(program, geoShader);
            if(!checkGlErr("fail to attach geometry shader"))
                goto err;
        }
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if(linkStatus != GL_TRUE)
        {
            LOG_ERR("fail to link program");
            logShaderInfo(program);
            glDeleteProgram(program);
            goto err;
        }
        LOG_DEBUG("succeed to build shader program");
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        if(compileGeo)
            glDeleteShader(geoShader);
        return program;

    err:
        if(vertShader)
            glDeleteShader(vertShader);
        if(fragShader)
            glDeleteShader(fragShader);
        if(geoShader)
            glDeleteShader(geoShader);
        return 0;
    }

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

    uint32_t GfxHelper::createShader(uint32_t type, const char *source)
    {
        //glCreateShader return zero when fail
        GLuint shader = glCreateShader(type);
        if(shader)
        {
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            GLint compileStatus = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
            logShaderInfo(shader);
            if(compileStatus != GL_TRUE)
            {
                glDeleteShader(shader);
                shader = 0;
            }
        }
        else
        {
            LOG_ERR("fail to create shader for type(0x%x), err code(0x%x)", type, glGetError());
        }
        return shader;
    }

    void GfxHelper::logShaderInfo(uint32_t shader)
    {
        if (shader == 0)
        {
            LOG_ERR("invalid input shader[0]");
            return;
        }
        GLint infoLen{0};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen)
        {
            char *info = static_cast<char *>(std::malloc(sizeof(char) * infoLen));
            if (info)
            {
                glGetShaderInfoLog(shader, infoLen, nullptr, info);
                LOG_DEBUG("shader(%u) status: %s", shader, info);
                std::free(info);
            }
            else
            {
                LOG_ERR("fail to malloc memory for shader(%u) info log", shader);
            }
        }
    }
}

