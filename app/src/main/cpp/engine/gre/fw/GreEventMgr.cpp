#include "GreEventMgr.h"
#include "SystemUtil.h"

namespace gre
{
    GreEventMgr::GreEventMgr() : m_evtQueues() {}

    GreEventMgr::~GreEventMgr() = default;

    void GreEventMgr::addEvtCb(GreEventType type, GreEventId id, PoolEvtArgType &&arg)
    {
        auto itr = m_evtQueues[type].find(id);
        if(itr != m_evtQueues[type].end())
        {
            itr->second.emplace_back(std::move(arg));
        }
        else
        {
            std::list<PoolEvtArgType> slotList;
            slotList.emplace_back(std::move(arg));
            m_evtQueues[type].insert(std::make_pair(id, std::move(slotList)));
        }
    }

    void GreEventMgr::process()
    {
        // 1. process all instant events, and remove them from list
        processAll(GreEventType::INSTANT, true);
        EVT_QUEUE_TYPE().swap(m_evtQueues[GreEventType::INSTANT]);

        // 2. process one UI event
        processOne(GreEventType::UI, true);

        // 3. process every render event
        processAll(GreEventType::RENDER, false);
    }

    void GreEventMgr::processAll(GreEventType type, bool remove)
    {
        if (!m_evtQueues[type].empty())
        {
            auto itr = m_evtQueues[type].begin();
            while (itr != m_evtQueues[type].end())
            {
                auto listItr = itr->second.begin();
                while(listItr != itr->second.end())
                {
                    PoolEvtArgType arg = std::move(*listItr);
                    GreObject *obj = arg->argObj;
                    if(obj)
                    {
                        int64_t start = systemTimeMs();
                        obj->slotCb(std::move(arg));
                        int64_t end = systemTimeMs();
                        if((end - start) > 1 * 1000)
                        {
                            LOG_WARN("event[type(%u) id(%u)] executing-time[%ld] exceeds 1s",
                                     type, itr->first, (end - start));
                        }
                    }
                    if (remove)
                    {
                        listItr = itr->second.erase(listItr);
                    }
                    else
                    {
                        listItr++;
                    }
                }
                if (remove)
                {
                    itr = m_evtQueues[type].erase(itr);
                }
                else
                {
                    itr++;
                }
            }
        }
    }

    void GreEventMgr::processOne(GreEventType type, bool remove)
    {
        if (!m_evtQueues[type].empty())
        {
            auto itr = m_evtQueues[type].begin();
            if (itr != m_evtQueues[type].end())
            {
                auto listItr = itr->second.begin();
                while(listItr != itr->second.end())
                {
                    PoolEvtArgType arg = std::move(*listItr);
                    GreObject *obj = arg->argObj;
                    if(obj)
                    {
                        int64_t start = systemTimeMs();
                        obj->slotCb(std::move(arg));
                        int64_t end = systemTimeMs();
                        if((end - start) > 1 * 1000)
                        {
                            LOG_WARN("event[type(%u) id(%u)] executing-time[%ld] exceeds 1s",
                                     type, itr->first, (end - start));
                        }
                    }
                    if (remove)
                    {
                        listItr = itr->second.erase(listItr);
                    }
                    else
                    {
                        listItr++;
                    }
                }
            }
            if (remove)
            {
                m_evtQueues[type].erase(itr);
            }
        }
    }

}
