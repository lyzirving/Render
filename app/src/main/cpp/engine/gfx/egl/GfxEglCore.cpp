#include <GLES3/gl3.h>

#include "include/GfxEglCore.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxEglCore"

namespace gfx
{
    GfxEglCore::GfxEglCore() : m_display(EGL_NO_DISPLAY),
                               m_context(EGL_NO_CONTEXT),
                               m_config(EGL_NO_CONFIG_KHR)
    {
    }

    GfxEglCore::~GfxEglCore()
    {
        release();
    }

    bool GfxEglCore::makeCurrent(EGLSurface draw, EGLSurface read)
    {
        bool success{false};
        if (m_context != EGL_NO_CONTEXT)
        {
            EGLBoolean ret = eglMakeCurrent(m_display, draw, read, m_context);
            if (ret != EGL_SUCCESS)
            {
                LOG_ERR("fail to make current, err[0x%x]", eglGetError());
            }
            else
            {
                success = true;
            }
        }
        else
        {
            LOG_ERR("egl context is null");
        }
        return success;
    }

    void GfxEglCore::release() {
        if (m_display != EGL_NO_DISPLAY && m_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(m_display, m_context);
            m_context = EGL_NO_CONTEXT;
            m_config = EGL_NO_CONFIG_KHR;
        }
        if (m_display != EGL_NO_DISPLAY)
        {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            eglTerminate(m_display);
            m_display = EGL_NO_DISPLAY;
        }
    }

    bool GfxEglCore::prepare(EGLContext shareContext)
    {
        bool success{false};

        EGLint major = 0;
        EGLint minor = 0;
        EGLint numConfig;
        EGLint version;

        EGLConfig config{nullptr};
        EGLDisplay display{EGL_NO_DISPLAY};
        EGLContext context{EGL_NO_CONTEXT};

        // hardcoded to RGBx output display
        const EGLint confAttr[] =
        {
                // tag                  value
                // EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_RECORDABLE_ANDROID, EGL_TRUE,
                EGL_NONE
        };

        // select OpenGL ES v3
        const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

        if (shareContext != EGL_NO_CONTEXT)
        {
            LOG_DEBUG("register shared context");
        }

        if (m_context != EGL_NO_CONTEXT)
        {
            LOG_DEBUG("context has been already created, quit");
            success = true;
            goto quit;
        }
        // set up OpenGL ES context associated with the default display
        // the display won't be visible
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY)
        {
            LOG_ERR("failed to get display, err[0x%x]", eglGetError());
            goto quit;
        }

        if (!eglInitialize(display, &major, &minor))
        {
            LOG_ERR("failed to initialize display, err code[0x%x]", eglGetError());
            goto quit;
        }

        LOG_DEBUG("display: version(%d.%d) vendor(%s)", major, minor, eglQueryString(display, EGL_VENDOR));

        // select the configuration that matches our desired characteristics best
        if (!eglChooseConfig(display, confAttr, &config, 1, &numConfig))
        {
            LOG_ERR("failed to find suitable config, err[0x%x]", eglGetError());
            goto quit;
        }

        context = eglCreateContext(display, config, shareContext, ctxAttr);
        if (context == EGL_NO_CONTEXT)
        {
            LOG_ERR("failed to create egl context, err[0x%x]", eglGetError());
            goto quit;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);

        eglQueryContext(display, context, EGL_CONTEXT_CLIENT_VERSION, &version);
        LOG_DEBUG("succeed to prepare egl core, information is listed as bellow:\n"
                  "****** EGL context client version %d\n"
                  "****** OpenGL ES version          %s\n"
                  "****** vendor                     %s\n"
                  "****** renderer                   %s",
                  version, glGetString(GL_VERSION), glGetString(GL_VENDOR),
                  glGetString(GL_RENDERER));

        success = true;

        quit:
        if (!success)
        {
            if (display != EGL_NO_DISPLAY && context != EGL_NO_CONTEXT)
            {
                eglDestroyContext(display, context);
            }
            if (display != EGL_NO_DISPLAY)
            {
                eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                eglTerminate(display);
            }
        }
        else
        {
            m_display = display;
            m_context = context;
            m_config = config;
        }
        return success;
    }

    void GfxEglCore::swapBuffer(bool block)
    {
        if (m_context != EGL_NO_CONTEXT)
        {
            if (block)
            {
                glFinish();
            }
            else
            {
                glFlush();
            }
        }
        else
        {
            LOG_ERR("context is null");
        }
    }
}



