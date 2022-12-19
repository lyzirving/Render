
#ifndef RENDER_GREEVENTMGR_H
#define RENDER_GREEVENTMGR_H

#include <unordered_map>
#include <list>

#include "GreObject.h"
#include "GreDef.h"

namespace gre
{
    //todo implement this class
    class GreEventMgr
    {
    public:
        GreEventMgr();
        virtual ~GreEventMgr();

        void addEvtCb(GreEventType type, GreEventId id, PoolEvtArgType &&arg);
        void process();

    private:
        using EVT_QUEUE_TYPE = std::unordered_map<uint8_t, std::list<PoolEvtArgType>>;

        void processAll(GreEventType type, bool remove = false);
        void processOne(GreEventType type, bool remove = false);

        EVT_QUEUE_TYPE m_evtQueues[GreEventType::EVT_TYPE_CNT];
    };
}

#endif //RENDER_GREEVENTMGR_H
