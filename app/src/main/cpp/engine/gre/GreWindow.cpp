#include <limits>

#include "GreWindow.h"
#include "GfxEglCore.h"
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
      mEgl(nullptr)
    {
    }

    GreWindow::~GreWindow() = default;

    bool GreWindow::attachSurface(ANativeWindow *surface)
    {
        if (!surface)
        {
            LOG_ERR("invalid surface ptr");
            return false;
        }
        //todo switch this event into rendering thread
        return true;
    }

    void GreWindow::slotCb(PoolEvtArgType &&arg)
    {
        printFps();
    }

    void GreWindow::slotCb(PoolSyncEvtArgType &&arg)
    {
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

}