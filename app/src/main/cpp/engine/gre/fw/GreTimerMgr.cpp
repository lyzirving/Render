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
    #define TIMEOUT_MS_MAX (30 * 1000)

    GreTimerMgr::GreTimerMgr() : m_timerArray(),
                                 m_tmpArray(), m_ticking(false),
                                 m_minExpiration(TIMEOUT_MS_MAX)
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
        timer->start();
        bool found{false};
        for (auto & item : m_timerArray)
        {
            if(timer.get() == item.get())
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
            if(m_ticking.load())
            {
                addWhenTicking(timer);
                return;
            }
            m_timerArray.emplace_back(timer);
            std::sort(m_timerArray.begin(), m_timerArray.end(), GreTimerCmpObj);
        }
    }

    void GreTimerMgr::addWhenTicking(const std::shared_ptr<GreTimer> &timer)
    {
        if (m_tmpArray.empty())
        {
            LOG_DEBUG("mgr is ticking, add timer[key%u, id%u, priority%u] to tmp array",
                      timer->getKey(), timer->getId(), timer->getPriority());
            m_tmpArray.push_back(timer);
        }
        else
        {
            bool found{false};
            auto itr = m_tmpArray.begin();
            while (itr != m_tmpArray.end())
            {
                if((*itr).get() == timer.get())
                {
                    LOG_DEBUG("mgr is ticking, and timer[key%u, id%u, priority%u] is already in tmp array",
                              timer->getKey(), timer->getId(), timer->getPriority());
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                m_tmpArray.push_back(timer);
            }
        }
    }

    void GreTimerMgr::computeMinExpiration()
    {
        m_minExpiration = TIMEOUT_MS_MAX;
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
        if(m_ticking.load())
        {
            LOG_DEBUG("timer mgr is ticking, stop it first");
            timer->stop();
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
        m_ticking.store(false);
        processTmpArray();
        computeMinExpiration();
    }

    void GreTimerMgr::processTimer()
    {
        auto itr = m_timerArray.begin();
        while(itr != m_timerArray.end())
        {
            GreTimer *timer = itr->get();
            if(timer)
            {
                if(timer->isRunning())
                {
                    int64_t timeMs = systemTimeMs();
                    if (timer->tick(timeMs))
                    {
                        PoolEvtArg arg = GreEventPool::get()->getEvtArg();
                        timer->fire(arg);
                    }
                    itr++;
                }
                else if (timer->isStopped())
                {
                    (*itr).reset();
                    itr = m_timerArray.erase(itr);
                }
                else
                {   // paused timer
                    itr++;
                }
            }
            else
            {
                itr = m_timerArray.erase(itr);
            }
        }
    }

    void GreTimerMgr::processTmpArray()
    {
        if (!m_tmpArray.empty())
        {
            auto itr = m_tmpArray.begin();
            while(itr != m_tmpArray.end())
            {
                m_timerArray.push_back(*itr);
                (*itr).reset();
                itr = m_tmpArray.erase(itr);
            }
            std::sort(m_timerArray.begin(), m_timerArray.end(), GreTimerCmpObj);
        }
    }
}
