#include <android/native_window_jni.h>

#include "GfxWindowSurface.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxWindowSurface"

namespace gfx
{
    GfxWindowSurface::GfxWindowSurface(const std::shared_ptr<GfxEglCore> &eglCore,
                                       ANativeWindow *window,
                                       uint8_t id) :
                                       m_eglCore(eglCore), m_window(window),
                                       m_surface(EGL_NO_SURFACE), m_id(id),
                                       m_width(0), m_height(0)
    {
    }

    GfxWindowSurface::~GfxWindowSurface() { release(); }

    bool GfxWindowSurface::isPrepared()
    {
        return m_surface != EGL_NO_SURFACE;
    }

    bool GfxWindowSurface::makeCurrent() {
        bool success{false};
        if (!m_eglCore)
        {
            LOG_ERR("[%u] egl core is null", m_id);
            goto done;
        }
        if (m_surface == EGL_NO_SURFACE)
        {
            LOG_ERR("[%u] egl surface is null", m_id);
            goto done;
        }
        if (m_eglCore->getDisplay() == EGL_NO_DISPLAY)
        {
            LOG_ERR("[%u] egl display is null", m_id);
            goto done;
        }
        if (m_eglCore->getContext() == EGL_NO_CONTEXT)
        {
            LOG_ERR("[%u] egl context is null", m_id);
            goto done;
        }
        if (!eglMakeCurrent(m_eglCore->getDisplay(), m_surface, m_surface, m_eglCore->getContext()))
        {
            LOG_ERR("[%u] fail to make current[0x%x]", m_id, eglGetError());
            goto done;
        }
        success = true;

    done:
        return success;
    }

    bool GfxWindowSurface::prepare()
    {
        if (isPrepared())
        {
            LOG_DEBUG("window surface[%u] already prepared", m_id);
            return true;
        }
        if (!m_eglCore)
        {
            LOG_ERR("window surface[%u]'s egl core is null", m_id);
            return false;
        }
        if (m_eglCore->getDisplay() == EGL_NO_DISPLAY)
        {
            LOG_ERR("[%u] empty egl display", m_id);
            return false;
        }
        if (!m_window)
        {
            LOG_ERR("[%u] window is null", m_id);
            return false;
        }

        const EGLint attribute[] = {EGL_NONE};
        m_surface = eglCreateWindowSurface(m_eglCore->getDisplay(),
                                           m_eglCore->getConfig(),
                                           m_window, attribute);
        EGLint ret = eglGetError();
        if (ret != EGL_SUCCESS)
        {
            LOG_ERR("[%u] fail to prepare, err code[0x%x]", m_id, ret);
            m_surface = EGL_NO_SURFACE;
        }
        else
        {
            m_width = ANativeWindow_getWidth(m_window);
            m_height = ANativeWindow_getHeight(m_window);
            LOG_DEBUG("[%u], success, width(%d) height(%d)", m_id, m_width, m_height);
        }
        return ret == EGL_SUCCESS;
    }

    void GfxWindowSurface::release()
    {
        if (m_surface != EGL_NO_SURFACE)
        {
            eglMakeCurrent(m_eglCore->getDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (eglDestroySurface(m_eglCore->getDisplay(), m_surface))
            {
                LOG_DEBUG("[%u] destroy surface", m_id);
            }
            else
            {
                LOG_ERR("[%u] fails to destroy surface[0x%x]", m_id, eglGetError());
            }
            m_eglCore.reset();
            m_surface = EGL_NO_SURFACE;
        }
        if (m_window)
        {
            LOG_DEBUG("[%u] release native window", m_id);
            ANativeWindow_release(m_window);
            m_window = nullptr;
        }
    }

    bool GfxWindowSurface::swapBuffer()
    {
        bool success{false};
        if (!m_eglCore)
        {
            LOG_ERR("[%u] egl core is null", m_id);
            goto done;
        }
        if (m_surface == EGL_NO_SURFACE)
        {
            LOG_ERR("[%u] egl surface is null", m_id);
            goto done;
        }
        if (m_eglCore->getDisplay() == EGL_NO_DISPLAY)
        {
            LOG_ERR("[%u] egl display is null", m_id);
            goto done;
        }
        if (!eglSwapBuffers(m_eglCore->getDisplay(), m_surface))
        {
            LOG_ERR("[%u] fail to swap buffer[0x%x]", m_id, eglGetError());
            goto done;
        }
        success = true;

    done:
        return success;
    }
}

