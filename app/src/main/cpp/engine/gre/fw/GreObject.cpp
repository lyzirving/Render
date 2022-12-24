#include "GreObject.h"
#include "GreContext.h"
#include "Sync.h"

namespace gre
{
    GreEventArg::GreEventArg() : m_type(GreEventType::EVT_TYPE_CNT),
                                 m_id(GreEventId::NONE),
                                 m_syncFlag(false), m_arg0(0), m_arg1(0),
                                 m_obj(nullptr), m_data(nullptr),
                                 m_sync(nullptr)
      {
      }

    GreEventArg::~GreEventArg()
    {
        clear();
        delete m_sync;
    }

    GreEventArg::GreEventArg(const GreEventArg &other)
    {
        this->m_type = other.m_type;
        this->m_id = other.m_id;
        this->m_syncFlag = other.m_syncFlag;
        this->m_arg0 = other.m_arg0;
        this->m_arg1 = other.m_arg1;
        this->m_obj = other.m_obj;
        this->m_data = other.m_data;
        this->m_sync = other.m_sync;
    }

    GreEventArg & GreEventArg::operator=(const GreEventArg &other)
    {
        if(this != &other)
        {
            this->m_type = other.m_type;
            this->m_id = other.m_id;
            this->m_syncFlag = other.m_syncFlag;
            this->m_arg0 = other.m_arg0;
            this->m_arg1 = other.m_arg1;
            this->m_obj = other.m_obj;
            this->m_data = other.m_data;
            this->m_sync = other.m_sync;
        }
        return *this;
    }

    GreEventArg::GreEventArg(GreEventArg &&other) noexcept
    {
        this->m_type = other.m_type;
        this->m_id = other.m_id;
        this->m_syncFlag = other.m_syncFlag;
        this->m_arg0 = other.m_arg0;
        this->m_arg1 = other.m_arg1;
        this->m_obj = other.m_obj;
        this->m_data = other.m_data;
        this->m_sync = other.m_sync;
        other.m_type = GreEventType::EVT_TYPE_CNT;
        other.m_id = GreEventId::NONE;
        other.m_obj = nullptr;
        other.m_data = nullptr;
        other.m_sync = nullptr;
    }

    GreEventArg & GreEventArg::operator=(GreEventArg &&other) noexcept
    {
        if(this != &other)
        {
            this->m_type = other.m_type;
            this->m_id = other.m_id;
            this->m_syncFlag = other.m_syncFlag;
            this->m_arg0 = other.m_arg0;
            this->m_arg1 = other.m_arg1;
            this->m_obj = other.m_obj;
            this->m_data = other.m_data;
            this->m_sync = other.m_sync;
            other.m_type = GreEventType::EVT_TYPE_CNT;
            other.m_id = GreEventId::NONE;
            other.m_obj = nullptr;
            other.m_data = nullptr;
            other.m_sync = nullptr;
        }
        return *this;
    }

    void GreEventArg::clear()
    {
        m_type = GreEventType::EVT_TYPE_CNT;
        m_id = GreEventId::NONE;
        m_arg0 = 0;
        m_arg1 = 0;
        m_obj = nullptr;
        m_data = nullptr;
        markSync(false);
    }

    void GreEventArg::markSync(bool flag)
    {
        m_syncFlag = flag;
        if(flag)
        {
            if (!m_sync)
            {
                m_sync = new Sync;
            }
        }
    }

    void GreEventArg::wrap(GreEventType type, GreEventId id)
    {
        m_type = type;
        m_id = id;
    }

    void GreEventArg::set(GreObject *obj, void *data)
    {
        m_obj = obj;
        m_data = data;
    }

    void GreEventArg::set(int32_t arg0, int32_t arg1)
    {
        m_arg0 = arg0;
        m_arg1 = arg1;
    }

    GreObject::GreObject() : m_ctx() {}

    GreObject::~GreObject()
    {
        m_ctx.reset();
    }

    void GreObject::slotCb(const PoolEvtArg &arg)
    {
        //no implementation in base class
    }

    std::shared_ptr<GreContext> GreObject::getCtx()
    {
        return m_ctx.lock();
    }

    void GreObject::setWeakCtx(const std::weak_ptr<GreContext> &ctx)
    {
        m_ctx = ctx;
    }
}

