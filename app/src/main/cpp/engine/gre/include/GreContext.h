#ifndef RENDER_GRECONTEXT_H
#define RENDER_GRECONTEXT_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   a context that contains all the resources needed for rendering
**/

#include <memory>
#include "GreDef.h"

struct ANativeWindow;

namespace gre
{
    class GreThread;
    class GreWindow;
    class GreTimerMgr;

    class GreContext
    {
    public:
        GreContext(GreContextId id = GreContextId::CTX_MAIN);
        virtual ~GreContext();

        bool attachSurface(ANativeWindow *surface);
        uint8_t init();
        void mainWork();
        void release();
        void setWeakSelf(const std::shared_ptr<GreContext> &context);

    private:

        std::weak_ptr<GreContext> m_self;
        GreContextId m_id;
        std::shared_ptr<GreThread> m_thread;
        std::shared_ptr<GreWindow> m_window;
        std::shared_ptr<GreTimerMgr> m_timerMgr;
    };
}

#endif //RENDER_GRECONTEXT_H
