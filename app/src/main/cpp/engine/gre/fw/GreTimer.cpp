#include "GreTimer.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimer"

namespace gre
{
    static uint32_t gTimerId{0};

    GreTimer::GreTimer(GreEventId key, int64_t intervalMs, GrePriority priority)
    : GreObject(),
      m_key(key), m_priority(priority),
      m_intervalMs(intervalMs), m_lastNotifyTime(0),
      m_isRunning(false), m_timerId(gTimerId++)
      {
      }

    GreTimer::~GreTimer() = default;

    bool GreTimer::operator==(const GreTimer &other) const
    {
        return m_timerId == other.m_timerId
               && m_key == other.m_key
               && m_priority == other.m_priority;
    }

    void GreTimer::fire(const PoolEvtArg &arg)
    {
        slotCb(arg);
    }

    void GreTimer::startTimer()
    {
        if(m_isRunning.load())
        {
            LOG_DEBUG("timer[%u] already start", m_key);
            return;
        }
        m_isRunning.store(true);
    }

    void GreTimer::stopTimer()
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

