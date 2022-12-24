#include <ctime>

#include "Sync.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Sync"

Sync::Sync() : m_mutex(), m_cond()
{
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}

Sync::~Sync()
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

void Sync::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void Sync::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

void Sync::signal()
{
    pthread_cond_signal(&m_cond);
}

void Sync::wait(int64_t timeoutUs)
{
    if (timeoutUs <= 0)
    {
        pthread_cond_wait(&m_cond, &m_mutex);
    }
    else
    {
        struct timeval now{};
        gettimeofday(&now, nullptr);

        struct timespec out{};
        out.tv_sec = now.tv_sec;
        out.tv_nsec = (now.tv_usec + timeoutUs) * 1000;

        pthread_cond_timedwait(&m_cond, &m_mutex, &out);
    }
}