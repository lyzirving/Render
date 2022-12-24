#include <cassert>
#include <unistd.h>
#include <sys/syscall.h>

#include "GreContext.h"
#include "GreThread.h"
#include "GreTimerMgr.h"
#include "GreWindow.h"
#include "GreEventMgr.h"
#include "GreEventPool.h"
#include "Sync.h"
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

    GreContext::GreContext(GreContextId id) : GreObject(),
                                              m_id(id),
                                              m_pTid(nullptr),
                                              m_kTid(),
                                              m_tMap(),
                                              m_thread(nullptr),
                                              m_window(nullptr),
                                              m_timerMgr(nullptr),
                                              m_evtMgr(nullptr)
    {
        pthread_key_create(&m_kTid, nullptr);
    }

    GreContext::~GreContext()
    {
        auto itr = m_tMap.begin();
        while (itr != m_tMap.end())
        {
            int64_t *p = itr->second;
            if(p) { free(p); }
            itr = m_tMap.erase(itr);
        }

        free(m_pTid);
        pthread_key_delete(m_kTid);

        m_window.reset();
        m_timerMgr.reset();
        m_thread.reset();
        m_evtMgr.reset();
    }

    bool GreContext::attachSurface(ANativeWindow *surface)
    {
        if (!m_window)
        {
            LOG_DEBUG("err, GreWindow is null");
            return false;
        }
        if (isMainThread())
        {
            return m_window->attachSurface(surface);
        }
        else
        {
            if(!m_evtMgr)
            {
                LOG_ERR("GreEventMrg is null");
                return false;
            }
            PoolEvtArg arg = GreEventPool::get()->getEvtArg();
            arg->wrap(GreEventType::INSTANT, GreEventId::ATTACH_SURFACE);
            arg->set(m_window.get(), surface);
            arg->markSync(true);
            m_evtMgr->addEvent(arg->type(), arg->id(), std::move(arg));
            return true;
        }
    }

    uint8_t GreContext::init()
    {
        if(m_ctx.expired())
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
        m_window->setWeakCtx(m_ctx);
        m_window->startTimer();

        m_timerMgr = std::make_shared<GreTimerMgr>();
        m_timerMgr->addTimer(m_window);

        m_evtMgr = std::make_shared<GreEventMgr>();

        m_thread = std::make_shared<GreThread>(CTX_ID_TO_STR(m_id));
        m_thread->setWeakCtx(m_ctx);
        m_thread->setFunc(threadLoop, this);
        m_thread->start();

        return GRE_SUCCESS;

    err:
        return GRE_ERROR;
    }

    bool GreContext::isMainThread()
    {
        if (!m_pTid)
        {
            LOG_ERR("main thread has not been started");
            assert(0);
        }
        auto *ptr = (int64_t *)pthread_getspecific(m_kTid);
        if (ptr == nullptr)
        {
            ptr = (int64_t *)std::malloc(sizeof(int64_t));
            *ptr = syscall(SYS_gettid);
            pthread_setspecific(m_kTid, ptr);
            m_tMap.insert(std::make_pair(*ptr, ptr));
        }
        return *ptr == *m_pTid;
    }

    void GreContext::mainWork()
    {
        if (!m_pTid)
        {
            m_pTid = (int64_t *)std::malloc(sizeof(int64_t));
            *m_pTid = syscall(SYS_gettid);
            pthread_setspecific(m_kTid, m_pTid);
            LOG_DEBUG("main thread id[%ld]", *m_pTid);
        }

        if (!m_evtMgr || !m_timerMgr)
        {
            LOG_ERR("event manager is null[%s] or timer manager is null[%s]",
                    m_evtMgr == nullptr ? "true" : "false",
                    m_timerMgr == nullptr ? "true" : "false");
            assert(0);
        }

        m_evtMgr->process(m_timerMgr->getTimeout());
        m_timerMgr->process();
    }

    void GreContext::requestQuit()
    {
        if (m_thread)
        {
            m_thread->interrupt();
            m_thread->join();
        }
    }

    void GreContext::release()
    {
        if (m_timerMgr)
            m_timerMgr->removeTimer(m_window);

        if (m_window)
            m_window->release();
    }

    void GreContext::slotCb(const PoolEvtArg &arg) {}

    void GreContext::testInterface()
    {

    }
}

