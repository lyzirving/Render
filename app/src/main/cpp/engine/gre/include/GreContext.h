#ifndef RENDER_GRECONTEXT_H
#define RENDER_GRECONTEXT_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   a context that contains all the resources needed for rendering
**/

#include <memory>
#include <mutex>
#include <pthread.h>
#include "GreDef.h"
#include "GreObject.h"

struct ANativeWindow;

namespace gre
{
    class GreThread;
    class GreWindow;
    class GreTimerMgr;

    class GreContext : public GreObject
    {
    public:
        GreContext(GreContextId id = GreContextId::CTX_MAIN);
        virtual ~GreContext();

        bool attachSurface(ANativeWindow *surface);
        uint8_t init();
        void mainWork();
        void requestQuit();
        void release();

    private:
        int64_t getThreadId();
        bool isMainThread();

        GreContextId m_id;
        int64_t m_mainThreadId;
        pthread_key_t m_keyThreadId;

        std::shared_ptr<GreThread> m_thread;
        std::shared_ptr<GreWindow> m_window;
        std::shared_ptr<GreTimerMgr> m_timerMgr;
    };
}

#endif //RENDER_GRECONTEXT_H
