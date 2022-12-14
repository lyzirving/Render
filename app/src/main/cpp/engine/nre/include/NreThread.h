#ifndef RENDER_NRETHREAD_H
#define RENDER_NRETHREAD_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   Thread for native rendering environment
**/

#include "Thread.h"

class NreThread : public Thread
{
public:
    NreThread(const char *name);
    virtual ~NreThread();

private:
};

#endif
