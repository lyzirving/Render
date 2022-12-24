#include "GreThread.h"
#include "GreContext.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreThread"

namespace gre
{
    GreThread::GreThread(const char *name) : Thread(name, true),
                                             GreObject(),
                                             m_pFunc(nullptr), m_arg(nullptr)
    {
    }

    GreThread::~GreThread() = default;

    void GreThread::onQuit()
    {
        LOG_DEBUG("enter");
        m_pFunc = nullptr;
        m_arg = nullptr;
        std::shared_ptr<GreContext> ctx = m_ctx.lock();
        if (ctx)
        {
            ctx->release();
        }
        else
        {
            LOG_ERR("context is null from weak ptr");
            assert(0);
        }
    }

    void GreThread::work()
    {
        if(!m_pFunc)
        {
            LOG_ERR("func ptr is null");
            interrupt();
        }
        else
        {
            m_pFunc(m_arg);
        }
    }

    void GreThread::setFunc(void (*pFunc)(void*), void *arg)
    {
        m_pFunc = pFunc;
        m_arg = arg;
    }

}