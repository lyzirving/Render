#include "GreEventMgr.h"
#include "SystemUtil.h"
#include "Sync.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreEventMgr"

namespace gre
{
    GreEventMgr::GreEventMgr() : m_evtQueues(), m_sync(new Sync) {}

    GreEventMgr::~GreEventMgr() = default;

    void GreEventMgr::addEvent(GreEventType type, GreEventId id, PoolEvtArg &&arg)
    {
        if(arg->syncFlag())
        {
            arg->m_sync->lock();
        }

        {
            m_sync->lock();
            auto itr = m_evtQueues[type].find(id);
            if(itr != m_evtQueues[type].end())
            {
                itr->second.push_back(arg);
            }
            else
            {
                std::list<PoolEvtArg> slotList;
                slotList.push_back(arg);
                m_evtQueues[type].insert(std::make_pair(id, std::move(slotList)));
            }
            m_sync->unlock();
        }

        if(arg->syncFlag())
        {
            arg->m_sync->wait();
            arg->m_sync->unlock();
        }
    }

    void GreEventMgr::process(int64_t timeoutMs)
    {
        m_sync->lock();
        m_sync->wait(timeoutMs * 1000);
        // 1. process all instant events, and remove them from list
        processAll(GreEventType::INSTANT, true);
        EVT_QUEUE_TYPE().swap(m_evtQueues[GreEventType::INSTANT]);

        // 2. process one UI event
        processOne(GreEventType::UI, true);

        // 3. process every render event
        processAll(GreEventType::RENDER, false);
        m_sync->unlock();
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
                    GreObject *obj = (*listItr)->m_obj;
                    if(obj)
                    {
                        if((*listItr)->syncFlag())
                        {
                            (*listItr)->m_sync->lock();
                        }
                        int64_t start = systemTimeMs();
                        PoolEvtArg arg = (*listItr);
                        obj->slotCb(arg);
                        int64_t end = systemTimeMs();
                        if((end - start) > 1 * 1000)
                        {
                            LOG_WARN("event[type(%u) id(%u)] executing-time[%ld] exceeds 1s",
                                     type, itr->first, (end - start));
                        }
                        if((*listItr)->syncFlag())
                        {
                            (*listItr)->m_sync->signal();
                            (*listItr)->m_sync->unlock();
                        }
                    }
                    if (remove)
                    {
                        (*listItr).reset();
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
                    GreObject *obj = (*listItr)->m_obj;
                    if(obj)
                    {
                        if((*listItr)->syncFlag())
                        {
                            (*listItr)->m_sync->lock();
                        }
                        int64_t start = systemTimeMs();
                        PoolEvtArg arg = (*listItr);
                        obj->slotCb(arg);
                        int64_t end = systemTimeMs();
                        if((end - start) > 1 * 1000)
                        {
                            LOG_WARN("event[type(%u) id(%u)] executing-time[%ld] exceeds 1s",
                                     type, itr->first, (end - start));
                        }
                        if((*listItr)->syncFlag())
                        {
                            (*listItr)->m_sync->signal();
                            (*listItr)->m_sync->unlock();
                        }
                    }
                    if (remove)
                    {
                        (*listItr).reset();
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
