#include <cassert>

#include "GreContext.h"
#include "GreThread.h"
#include "GreWindow.h"
#include "GreTimerManager.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreContext"

namespace gre
{
    static void loopWork(void *arg)
    {
        if(!arg)
        {
            auto* ctx = static_cast<GreContext *>(arg);
            ctx->main();
        }
    }

    GreContext::GreContext(GreContextId id) : m_id(id),
                                              m_thread(nullptr),
                                              m_window(nullptr),
                                              m_timerMgr(nullptr)
    {
    }

    GreContext::~GreContext() = default;

    uint8_t GreContext::init()
    {
        if(m_id >= GreContextId::CTX_COUNT)
        {
            LOG_ERR("invalid context id[%u]", m_id);
            goto err;
        }

        m_window = std::make_shared<GreWindow>(m_id);
        m_window->start();

        m_timerMgr = std::make_shared<GreTimerManager>();
        m_timerMgr->addTimer(m_window);

        m_thread = std::make_shared<GreThread>(CTX_ID_TO_STR(m_id));
        m_thread->setFunc(loopWork, this);
        m_thread->start();

        return GRE_SUCCESS;

    err:
        return GRE_ERROR;
    }

    void GreContext::main()
    {
        if (m_timerMgr)
        {
            m_timerMgr->process();
        }
        else
        {
            LOG_ERR("timer manager is null");
            assert(0);
        }
    }
}

