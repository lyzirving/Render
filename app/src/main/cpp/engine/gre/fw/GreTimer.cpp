#include "GreTimer.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimer"

namespace gre
{
    static uint32_t gTimerId{0};

    GreTimer::GreTimer(GreEventId evtId, int64_t intervalMs, GrePriority priority)
    : GreObject(),
      m_evtId(evtId), m_priority(priority),
      m_intervalMs(intervalMs), m_lastNotifyTime(0),
      m_timerId(gTimerId++), m_status(Status::IDLE)
      {
      }

    GreTimer::~GreTimer() = default;

    bool GreTimer::operator==(const GreTimer &other) const
    {
        return m_timerId == other.m_timerId
               && m_evtId == other.m_evtId
               && m_priority == other.m_priority;
    }

    void GreTimer::fire(const PoolEvtArg &arg)
    {
        slotCb(arg);
    }

    bool GreTimer::tick(int64_t sysTimeMs)
    {
        if(!isRunning())
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

