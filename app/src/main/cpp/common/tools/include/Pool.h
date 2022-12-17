#ifndef RENDER_POOL_H
#define RENDER_POOL_H

#include <functional>
#include <memory>
#include <mutex>
#include <list>
#include "LogUtil.h"

template <class T>
class Pool
{
public:
    using Deleter = std::function<void(T*)>;

    Pool(uint32_t max = 50) : m_pool(), m_max(max) {};
    virtual ~Pool() { release(); }

    bool empty()
    {
        return m_pool.empty();
    }

    std::unique_ptr<T, Deleter> get()
    {
        Deleter deleter = [this](T *t)->void { add(t); };

        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_pool.empty())
        {
            m_pool.push_back(std::unique_ptr<T>(new T));
        }
        std::unique_ptr<T, Deleter> item(m_pool.back().release(), deleter);
        m_pool.pop_back();
        return std::move(item);
    }

    std::uint32_t size()
    {
        return m_pool.size();
    }

    void release()
    {
        if(!m_pool.empty())
        {
            auto itr = m_pool.begin();
            while(itr != m_pool.end())
            {
                std::unique_ptr<T> ptr(m_pool.back().release());
                ptr.reset();
                itr = m_pool.erase(itr);
            }
        }
    }

private:
    void add(T *item)
    {
        if(!item)
        {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            uint32_t size = m_pool.size();
            if(size < m_max)
            {
                __android_log_print(ANDROID_LOG_DEBUG, LIB_TAG,
                                    "[Pool][add] current pool size[%u]",
                                    (size + 1));
                std::unique_ptr<T> newItem(item);
                m_pool.push_back(std::move(newItem));
            }
            else
            {
                __android_log_print(ANDROID_LOG_DEBUG, LIB_TAG,
                                    "[Pool][add] out of max size[%u]",
                                    m_max);
                delete item;
            }
        }
    }

    std::list<std::unique_ptr<T>> m_pool;
    uint32_t m_max;
    std::mutex m_mutex;
};

#endif //RENDER_POOL_H
