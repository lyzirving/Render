#include <limits>
#include <android/native_window_jni.h>

#include "GreWindow.h"
#include "../render/include/GreSceneRender.h"
#include "../render/include/GreRrtRender.h"
#include "../render/include/GreBVHRender.h"

#include "GfxEglCore.h"
#include "GfxWindowSurface.h"
#include "GfxShaderMgr.h"

#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreWindow"

namespace gre
{
    GreWindow::GreWindow(GreContextId id)
    : GreTimer(GreEventId::REFRESH, 1000 / 60, GrePriority::TOP),
      m_id(id), m_totalFrame(0), m_lastRecTimeMs(0), m_fps(0),
      m_render(nullptr), m_egl(nullptr), m_surface(nullptr)
    {
    }

    GreWindow::~GreWindow()
    {
        m_surface.reset();
        m_egl.reset();
        m_render.reset();
    }

    bool GreWindow::attachSurface(ANativeWindow *surface)
    {
        if (!surface)
        {
            LOG_ERR("invalid surface ptr");
            return false;
        }

        if(!m_egl->isPrepared())
        {
            LOG_DEBUG("egl is not prepared");
            assert(0);
        }
        if(m_surface && m_surface->isPrepared())
        {
            LOG_ERR("window surface already exists");
            return false;
        }
        m_surface = std::make_shared<gfx::GfxWindowSurface>(m_egl, surface, m_id);
        return m_surface->prepare();
    }

    void GreWindow::detachSurface()
    {
        m_surface.reset();
    }

    void GreWindow::slotCb(const PoolEvtArg &arg)
    {
        switch (arg->id())
        {
            case GreEventId::REFRESH:
            {
                update();
                break;
            }
            case GreEventId::ATTACH_SURFACE:
            {
                LOG_DEBUG("event attach surface");
                auto *window =  (ANativeWindow *)arg->data();
                attachSurface(window);
                break;
            }
            case GreEventId::DETACH_SURFACE:
            {
                LOG_DEBUG("event detach surface");
                detachSurface();
                break;
            }
            default:
            {
                break;
            }
        }
    }

    bool GreWindow::prepare()
    {
        bool ret{false};
        m_egl = std::make_shared<gfx::GfxEglCore>();
        ret = m_egl->prepare();
        gfx::GfxShaderMgr::get()->init();
        if (m_id == GreContextId::CTX_MAIN)
        {
            m_render = std::make_shared<GreSceneRender>();
        }
        else if (m_id == GreContextId::CTX_SUB)
        {
            m_render = std::make_shared<GreRrtRender>();
        }
        else
        {
            m_render = std::make_shared<GreBVHRender>();
        }
        return ret;
    }

    void GreWindow::printFps()
    {
        if (m_totalFrame == ULONG_MAX - 1)
        {
            m_totalFrame = 0;
        }
        else
        {
            m_totalFrame++;
        }
        m_fps++;

        if (m_lastRecTimeMs == 0)
        {
            m_lastRecTimeMs = systemTimeMs();
        }
        else
        {
            int64_t cur = systemTimeMs();
            if (cur - m_lastRecTimeMs > 1000)
            {
                m_lastRecTimeMs = cur;
                LOG_DEBUG("fps[%u], frames[%lu]", m_fps, m_totalFrame);
                m_fps = 0;
            }
        }
    }

    void GreWindow::release()
    {
        if(m_surface)
        {
            m_surface->release();
            m_surface.reset();
        }

        if(m_render)
        {
            m_render.reset();
        }

        gfx::GfxShaderMgr::get()->release();

        if(m_egl)
        {
            m_egl->release();
            m_egl.reset();
        }
    }

    bool GreWindow::renderValid()
    {
        return m_egl != nullptr && m_surface != nullptr && m_render != nullptr;
    }

    void GreWindow::update()
    {
        if (renderValid())
        {
            onBeginRender();
            onRender();
            onEndRender();
        }
        printFps();
    }

    void GreWindow::onBeginRender()
    {
        m_surface->makeCurrent();
        m_render->setViewport(0, 0, m_surface->getWidth(), m_surface->getHeight());
        m_render->preUpd();
    }

    void GreWindow::onRender()
    {
        m_render->update();
    }

    void GreWindow::onEndRender()
    {
        m_render->postUpd();
        m_surface->swapBuffer();
    }

}