
#ifndef RENDER_GREEVENTMGR_H
#define RENDER_GREEVENTMGR_H

#include <unordered_map>
#include <list>

#include "GreObject.h"
#include "GreDef.h"

class Sync;

namespace gre
{
    class GreEventMgr
    {
    public:
        GreEventMgr();
        virtual ~GreEventMgr();

        void addEvent(GreEventType type, GreEventId id, PoolEvtArg &&arg);
        void process(int64_t timeoutMs);

    private:
        using EVT_QUEUE_TYPE = std::unordered_map<uint8_t, std::list<PoolEvtArg>>;

        void processAll(GreEventType type, bool remove = false);
        void processOne(GreEventType type, bool remove = false);

        EVT_QUEUE_TYPE m_evtQueues[GreEventType::EVT_TYPE_CNT];
        std::shared_ptr<Sync> m_sync;
    };
}

#endif //RENDER_GREEVENTMGR_H
