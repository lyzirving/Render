#ifndef RENDER_SYNC_H
#define RENDER_SYNC_H

#include <pthread.h>

class Sync
{
public:
    Sync();
    virtual ~Sync();

    void lock();
    void unlock();
    void signal();
    void wait(int64_t timeoutUs = 0);

private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif //RENDER_SYNC_H
