#ifndef RENDER_GRETHREAD_H
#define RENDER_GRETHREAD_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   Thread for graphics rendering environment
**/

#include <memory>

#include "Thread.h"
#include "GreObject.h"

namespace gre
{
    class GreThread : public Thread, public GreObject
    {
    public:
        GreThread(const char *name);

        virtual ~GreThread();
        virtual void onQuit() override;
        virtual void work() override;

        void setFunc(void (*pFunc)(void*), void *arg);

    private:
        void (*m_pFunc)(void*);
        void* m_arg;
    };
}

#endif
