#include "GreThread.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreThread"

namespace gre
{
    GreThread::GreThread(const char *name) : Thread(name, true),
                                             m_pFunc(nullptr), m_arg(nullptr)
    {
    }

    GreThread::~GreThread() = default;

    void GreThread::onQuit()
    {
        m_pFunc = nullptr;
        m_arg = nullptr;
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