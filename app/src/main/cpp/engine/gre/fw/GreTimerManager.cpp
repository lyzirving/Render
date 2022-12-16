#include <algorithm>

#include "GreTimerManager.h"
#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimerManager"

namespace gre
{
    GreTimerManager::GreTimerManager() : m_timerList()
    {
    }

    GreTimerManager::~GreTimerManager()
    {
        if(!m_timerList.empty())
        {
            auto itr = m_timerList.begin();
            while (itr != m_timerList.end())
            {
                itr->reset();
                itr = m_timerList.erase(itr);
            }
        }
    }

    void GreTimerManager::addTimer(const std::shared_ptr<GreTimer> &timer)
    {
        m_timerList.emplace_back(timer);
        std::sort(m_timerList.begin(), m_timerList.end(), GreTimerCmpObj);
    }

    void GreTimerManager::process()
    {
        auto itr = m_timerList.begin();
        while(itr != m_timerList.end())
        {
            GreTimer *timer = itr->get();
            if(timer)
            {
                int64_t timeMs = systemTimeMs();
                if (timer->tick(timeMs))
                {
                    //todo make event poll
                }
                itr++;
            }
            else
            {
                itr = m_timerList.erase(itr);
            }
        }
    }
}
