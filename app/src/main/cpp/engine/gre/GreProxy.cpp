#include "GreProxy.h"
#include "GreContext.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreProxy"

namespace gre {
    static std::shared_ptr<GreContext> m_arrayCtx[GreContextId::CTX_COUNT];

    void PROXY_attachView(int32_t id) {
        if (id >= GreContextId::CTX_COUNT || id < 0) {
            LOG_ERR("invalid input id[%d], context count[%u]", id, GreContextId::CTX_COUNT);
            return;
        }
        if (!m_arrayCtx[id]) {
            m_arrayCtx[id] = std::make_shared<GreContext>(GreContextId(id));
            std::shared_ptr<GreContext> self(m_arrayCtx[id]);
            m_arrayCtx[id]->setWeakSelf(self);
            if(m_arrayCtx[id]->init() == GRE_SUCCESS)
            {
                LOG_DEBUG("success to attach view[%u]", id);
            }
            else
            {
                LOG_ERR("fail to attach view[%u]", id);
                PROXY_detachView(id);
            }
        }
        else
        {
            LOG_ERR("view[%u] already existed", id);
        }
    }

    bool PROXY_attachSurface(int32_t id, ANativeWindow *window)
    {
        if(id >= GreContextId::CTX_COUNT || id < 0)
        {
            LOG_ERR("invalid input id[%d], context count[%u]", id, GreContextId::CTX_COUNT);
            return false;
        }
        if(!window)
        {
            LOG_ERR("window ptr is null");
            return false;
        }
        if(!m_arrayCtx[id])
        {
            LOG_ERR("gre context is invalid");
            return false;
        }
        return m_arrayCtx[id]->attachSurface(window);
    }

    void PROXY_detachView(int32_t id)
    {
        if(id >= GreContextId::CTX_COUNT || id < 0)
        {
            LOG_ERR("invalid input id[%d], context count[%u]", id, GreContextId::CTX_COUNT);
            return;
        }
        if(m_arrayCtx[id])
        {
            m_arrayCtx[id]->release();
            m_arrayCtx[id].reset();
            LOG_DEBUG("success to detach view[%u]", id);
        }
    }
}

