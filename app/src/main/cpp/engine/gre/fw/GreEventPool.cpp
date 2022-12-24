#include <mutex>

#include "GreEventPool.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreEventPool"

namespace gre
{
    static std::mutex gMutex{};
    static GreEventPool *gPtr{nullptr};

    GreEventPool * GreEventPool::get()
    {
        if(!gPtr)
        {
            std::lock_guard<std::mutex> lock(gMutex);
            if(!gPtr)
            {
                gPtr = new GreEventPool;
            }
        }
        return gPtr;
    }

    void GreEventPool::release(GreEventPool *ptr)
    {
        delete ptr;
    }

    GreEventPool::GreEventPool() : m_poolEvtArg() {}

    GreEventPool::~GreEventPool()
    {
        m_poolEvtArg.release();
    }

    PoolEvtArg GreEventPool::getEvtArg()
    {
        return m_poolEvtArg.get();
    }
}



