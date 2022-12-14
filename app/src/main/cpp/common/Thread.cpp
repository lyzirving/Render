#include "Thread.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Thread"

Thread::Thread(const char* name, bool loop) : m_interrupted(false),
                                              m_isRunning(false),
                                              m_loop(loop),
                                              m_thread(),
                                              m_name(name)
{
}

Thread::~Thread()
{
    if(!m_interrupted.load())
    {
        m_interrupted.store(true);
    }

    if(m_thread.joinable())
    {
        m_thread.join();
    }
}

void Thread::interrupt()
{
    m_interrupted.store(true);
}

bool Thread::isInterrupted()
{
    return m_interrupted.load();
}

void Thread::join()
{
    if(m_thread.joinable())
    {
        m_thread.join();
    }
}

void Thread::run()
{
    m_isRunning.store(true);
    LOG_DEBUG("thread[%s] start", m_name.c_str());
    if (m_loop)
    {
        while(!m_interrupted.load())
        {
            work();
        }
    }
    else
    {
        work();
    }
    m_isRunning.store(false);
    LOG_DEBUG("thread[%s] quit", m_name.c_str());
}

void Thread::start()
{
    if(!m_isRunning.load())
    {
        std::thread thread(std::bind(&Thread::run, this));
        m_thread = std::move(thread);
    }
    else
    {
        LOG_DEBUG("thread[%s] is already running", m_name.c_str());
    }
}

