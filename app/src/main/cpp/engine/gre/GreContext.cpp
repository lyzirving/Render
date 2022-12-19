#include <cassert>
#include <unistd.h>
#include <sys/syscall.h>

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
                                              m_timerMgr(nullptr),
                                              m_keyThreadId(),
                                              m_mainThreadId(0)
    {
        pthread_key_create(&m_keyThreadId, nullptr);
    }

    GreContext::~GreContext()
    {
        m_self.reset();
        m_window.reset();
        m_timerMgr.reset();
        m_thread.reset();
        pthread_key_delete(m_keyThreadId);
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

    int64_t GreContext::getThreadId()
    {
        int64_t *ptr;
        ptr = (int64_t *)pthread_getspecific(m_keyThreadId);
        if (ptr == nullptr)
        {
            ptr = (int64_t*)std::malloc(sizeof(int64_t));
            *ptr = syscall(SYS_gettid);
            pthread_setspecific(m_keyThreadId, ptr);
        }
        return *ptr;
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
        m_window->setWeakCtx(m_self);
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

    bool GreContext::isMainThread()
    {
        if (m_mainThreadId == 0)
        {
            LOG_ERR("main thread has not been started");
            assert(0);
        }
        return getThreadId() == m_mainThreadId;
    }

    void GreContext::mainWork()
    {
        if (m_mainThreadId == 0)
        {
            m_mainThreadId = getThreadId();
            LOG_DEBUG("main thread id[%ld]", m_mainThreadId);
        }
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
        if (isMainThread())
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
        else
        {
            LOG_DEBUG("not in rendering thread");
            //todo switch it in rendering thread and block
        }
    }

    void GreContext::setWeakSelf(const std::shared_ptr<GreContext> &context)
    {
        m_self = context;
    }
}

