#include <algorithm>
#include <limits>

#include "GreTimerMgr.h"
#include "GreEventPool.h"
#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimerMgr"

namespace gre
{
    #define TIMEOUT_INFINITE LONG_MAX

    GreTimerMgr::GreTimerMgr()
    : m_timerArray(), m_tmpArray(), m_ticking(false), m_minExpiration(TIMEOUT_INFINITE)
    {
    }

    GreTimerMgr::~GreTimerMgr()
    {
        if(!m_timerArray.empty())
        {
            auto itr = m_timerArray.begin();
            while (itr != m_timerArray.end())
            {
                itr->reset();
                itr = m_timerArray.erase(itr);
            }
        }
        if(!m_tmpArray.empty())
        {
            auto itr = m_tmpArray.begin();
            while (itr != m_tmpArray.end())
            {
                itr->reset();
                itr = m_tmpArray.erase(itr);
            }
        }
    }

    void GreTimerMgr::addTimer(const std::shared_ptr<GreTimer> &timer)
    {
        if (!timer)
        {
            return;
        }
        bool found{false};
        for (auto & item : m_timerArray) {
            if((*timer) == (*item))
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            LOG_DEBUG("timer[id[%u], key[%u], priority[%u]] has already been added",
                      timer->getId(), timer->getKey(), timer->getPriority());
        }
        else
        {
            m_timerArray.emplace_back(timer);
            std::sort(m_timerArray.begin(), m_timerArray.end(), GreTimerCmpObj);
        }
    }

    void GreTimerMgr::computeMinExpiration()
    {
        m_minExpiration = TIMEOUT_INFINITE;
        if(!m_timerArray.empty())
        {
            auto itr = m_timerArray.begin();
            while (itr != m_timerArray.end())
            {
                int64_t expire = itr->get()->getExpiration();
                if(expire < m_minExpiration)
                {
                    m_minExpiration = expire;
                }
                itr++;
            }
        }
    }

    void GreTimerMgr::removeTimer(const std::shared_ptr<GreTimer> &timer)
    {
        if (!timer)
        {
            return;
        }
        bool found{false};
        uint32_t ind{0};
        for (auto & item : m_timerArray) {
            if((*timer) == (*item))
            {
                found = true;
                break;
            }
            ind++;
        }
        if (found && ind < m_timerArray.size())
        {
            LOG_DEBUG("timer[id[%u], key[%u], priority[%u]] is removed",
                      timer->getId(), timer->getKey(), timer->getPriority());
            m_timerArray[ind].reset();
            m_timerArray.erase(m_timerArray.begin() + ind, m_timerArray.begin() + ind + 1);
        }
    }

    void GreTimerMgr::process()
    {
        m_ticking.store(true);
        processTimer();
        //todo logic: process timer-add motion when manager is ticking
        computeMinExpiration();
        m_ticking.store(false);
    }

    void GreTimerMgr::processTimer()
    {
        auto itr = m_timerArray.begin();
        while(itr != m_timerArray.end())
        {
            GreTimer *timer = itr->get();
            if(timer)
            {
                if(timer->isStart())
                {
                    int64_t timeMs = systemTimeMs();
                    if (timer->tick(timeMs))
                    {
                        timer->fire(std::move(GreEventPool::get()->getEvtArg()));
                    }
                }
                itr++;
            }
            else
            {
                itr = m_timerArray.erase(itr);
            }
        }
    }
}
