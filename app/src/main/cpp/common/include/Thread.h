#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

/**
 * @author  lyzirving
 * @date    2022-12-13
 * @brief   common class for c++ thread
**/

#include <thread>
#include <string>
#include <atomic>

class Thread
{
public:
    Thread(const char* name, bool loop = true);

    virtual ~Thread();
    virtual void onQuit() = 0;
    virtual void work() = 0;

    void interrupt();
    bool isInterrupted();
    void join();
    void start();

protected:
    void run();

protected:
    std::atomic<bool> m_interrupted;
    std::atomic<bool> m_isRunning;
    bool m_loop;
    std::thread m_thread;
    std::string m_name;
};

#endif //RENDER_THREAD_H
