#include <cassert>

#include "GreContext.h"
#include "GreThread.h"
#include "GreTimerMgr.h"
#include "GreWindow.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreContext"

namespace gre {

    static void threadLoop(void *arg) {
        if (arg)
        {
            auto *ctx = static_cast<GreContext *>(arg);
            ctx->mainWork();
        }
    }

    GreContext::GreContext(GreContextId id) : m_self(),
                                              m_id(id),
                                              m_thread(nullptr),
                                              m_window(nullptr),
                                              m_timerMgr(nullptr) {
    }

    GreContext::~GreContext()
    {
        m_self.reset();
        m_window.reset();
        m_timerMgr.reset();
        m_thread.reset();
    }

    bool GreContext::attachSurface(ANativeWindow *surface)
    {
        if(m_window)
        {
            return m_window->attachSurface(surface);
        }
        else
        {
            LOG_ERR("GreWindow is null");
            return false;
        }
    }

    uint8_t GreContext::init()
    {
        if(m_self.expired())
        {
            LOG_ERR("context is invalid or weak self has not been set");
            goto err;
        }

        if(m_id >= GreContextId::CTX_COUNT)
        {
            LOG_ERR("invalid context id[%u]", m_id);
            goto err;
        }

        m_window = std::make_shared<GreWindow>(m_id);
        m_window->startTimer();

        m_timerMgr = std::make_shared<GreTimerMgr>();
        m_timerMgr->addTimer(m_window);

        m_thread = std::make_shared<GreThread>(CTX_ID_TO_STR(m_id));
        m_thread->setFunc(threadLoop, this);
        m_thread->start();

        return GRE_SUCCESS;

    err:
        return GRE_ERROR;
    }

    void GreContext::mainWork()
    {
        if (m_timerMgr)
        {
            m_timerMgr->process();
        }
        else
        {
            LOG_ERR("timer manager is null");
            assert(0);
        }
    }

    void GreContext::release()
    {
        if(m_window)
            m_window->stopTimer();

        if (m_timerMgr)
            m_timerMgr->removeTimer(m_window);

        if (m_thread)
        {
            m_thread->interrupt();
            m_thread->join();
        }
    }

    void GreContext::setWeakSelf(const std::shared_ptr<GreContext> &context)
    {
        m_self = context;
    }
}

