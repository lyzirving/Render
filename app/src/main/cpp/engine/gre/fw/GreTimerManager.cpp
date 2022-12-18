#include <algorithm>

#include "GreTimerManager.h"
#include "GreEventPool.h"
#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimerManager"

namespace gre
{
    GreTimerManager::GreTimerManager() : m_timerArray()
    {
    }

    GreTimerManager::~GreTimerManager()
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
    }

    void GreTimerManager::addTimer(const std::shared_ptr<GreTimer> &timer)
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

    void GreTimerManager::removeTimer(const std::shared_ptr<GreTimer> &timer)
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

    void GreTimerManager::process()
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
                    itr++;
                }
            }
            else
            {
                itr = m_timerArray.erase(itr);
            }
        }
    }
}
