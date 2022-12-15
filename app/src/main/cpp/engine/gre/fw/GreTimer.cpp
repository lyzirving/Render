#include "GreTimer.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimer"

namespace gre
{
    GreTimer::GreTimer(GreTimerKey key, int64_t intervalMs, GrePriority priority)
    : GreObject(),
      m_key(key), m_priority(priority),
      m_intervalMs(intervalMs), m_lastNotifyTime(0),
      m_isRunning(false)
    {
    }

    GreTimer::~GreTimer() = default;

    void GreTimer::fire(std::shared_ptr<GreEventArg> &arg)
    {
        slotCallback(arg);
    }

    void GreTimer::start()
    {
        if(m_isRunning.load())
        {
            LOG_DEBUG("timer[%u] already start", m_key);
            return;
        }
        m_isRunning.store(true);
    }

    void GreTimer::stop()
    {
        m_isRunning.store(false);
    }

    bool GreTimer::tick(int64_t sysTimeMs)
    {
        if(!m_isRunning.load())
        {
            return false;
        }
        if(sysTimeMs < getExpiration())
        {
            return false;
        }
        m_lastNotifyTime = sysTimeMs;
        return true;
    }

}

