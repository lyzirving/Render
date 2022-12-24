#ifndef RENDER_GREOBJECT_H
#define RENDER_GREOBJECT_H

#include "Pool.h"
#include "GreDef.h"

class Sync;

namespace gre
{
    class GreObject;
    class GreContext;
    class GreEventMgr;

    class GreEventArg
    {
    public:
        friend class GreEventMgr;

        GreEventArg();
        virtual ~GreEventArg();

        GreEventArg(const GreEventArg &other);
        GreEventArg & operator=(const GreEventArg &other);

        GreEventArg(GreEventArg &&other) noexcept;
        GreEventArg & operator=(GreEventArg &&other) noexcept;

        void clear();
        void markSync(bool flag = true);
        void wrap(GreEventType type, GreEventId id);
        void set(GreObject *obj, void *data = nullptr);
        void set(int32_t arg0, int32_t arg1);

        inline GreEventType type() { return m_type; }
        inline GreEventId id() { return m_id; }
        inline bool syncFlag() { return m_syncFlag; }

    private:
        GreEventType m_type;
        GreEventId m_id;

        bool m_syncFlag;
        int32_t m_arg0, m_arg1;

        GreObject* m_obj;
        void *m_data;

        Sync *m_sync;
    };

    using PoolEvtArg = std::shared_ptr<GreEventArg>;

    class IEvent
    {
    public:
        IEvent() {}
        virtual ~IEvent() {}

        virtual void fire(const PoolEvtArg &arg) = 0;
    };

    class GreObject
    {
    public:
        GreObject();

        virtual ~GreObject();
        virtual void slotCb(const PoolEvtArg &arg);

        std::shared_ptr<GreContext> getCtx();
        void setWeakCtx(const std::weak_ptr<GreContext> &ctx);

    protected:
        std::weak_ptr<GreContext> m_ctx;
    };
}

#endif //RENDER_GREOBJECT_H
