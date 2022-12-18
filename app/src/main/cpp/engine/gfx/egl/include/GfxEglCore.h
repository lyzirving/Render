#ifndef RENDER_GFXEGLCORE_H
#define RENDER_GFXEGLCORE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace gfx
{
    class GfxEglCore {
    public:
        GfxEglCore();
        ~GfxEglCore();

        inline EGLDisplay getDisplay() { return m_display; }
        inline EGLConfig  getConfig()  { return m_config; }
        inline EGLContext getContext() { return m_context; }

        bool makeCurrent(EGLSurface draw = EGL_NO_SURFACE, EGLSurface read = EGL_NO_SURFACE);
        void release();
        bool prepare(EGLContext shareContext = EGL_NO_CONTEXT);
        void swapBuffer(bool block = false);

    private:
        EGLDisplay m_display;
        EGLContext m_context;
        EGLConfig m_config;
    };
}

#endif
