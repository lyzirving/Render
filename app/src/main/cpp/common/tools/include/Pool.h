#ifndef RENDER_POOL_H
#define RENDER_POOL_H

#include <functional>
#include <memory>
#include <mutex>
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Pool"

#define POOL_LOG_D(format, ...)  __android_log_print(ANDROID_LOG_DEBUG,               \
                                                     LIB_TAG, "[%s][%s] " format,     \
                                                     LOCAL_TAG, __FUNCTION__,         \
                                                     ##__VA_ARGS__)

#define POOL_LOG_W(format, ...)  __android_log_print(ANDROID_LOG_WARN,                \
                                                     LIB_TAG, "[%s][%s] " format,     \
                                                     LOCAL_TAG, __FUNCTION__,         \
                                                     ##__VA_ARGS__)

#define POOL_LOG_E(format, ...)  __android_log_print(ANDROID_LOG_ERROR,               \
                                                     LIB_TAG, "[%s][%s] " format,     \
                                                     LOCAL_TAG, __FUNCTION__,         \
                                                     ##__VA_ARGS__)

template <class T>
class Pool
{
public:
    Pool(int32_t max = 70, int32_t capacity = 8) : m_max(max),
                                                   m_size(0),
                                                   m_capacity(capacity),
                                                   m_mutex()
    {
        m_pool = (std::shared_ptr<T> *)std::malloc(m_capacity * sizeof(std::shared_ptr<T>));
        std::memset(m_pool, 0, m_capacity * sizeof(std::shared_ptr<T>));
    }
    virtual ~Pool() { release(); }

    bool empty() { return m_size == 0; }

    std::int32_t size() { return m_size; }

    bool valid() { return m_capacity <= m_max && m_size <= m_capacity && m_pool; }

    void release()
    {
        for (int i = 0; i < m_size; ++i)
        {
            m_pool[i].reset();
        }
        std::free(m_pool);
    }

    std::shared_ptr<T> get()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::shared_ptr<T> result;
        int32_t ind = idle();
        if (ind >= 0)
        {
            m_pool[ind]->clear();
            result = m_pool[ind];
        }
        else if(m_capacity < m_max)
        {
            // need more space
            resize(m_capacity * 2);
            ind = idle();
            m_pool[ind]->clear();
            result = m_pool[ind];
        }
        else
        {
            POOL_LOG_E("current cap[%d] is out of limit[%d]", m_capacity, m_max);
        }
        return result;
    }

private:
    int32_t idle()
    {
        int32_t ind = -1;
        for (int i = 0; i < m_size; ++i) {
            if(m_pool[i].use_count() == 1)
            {
                ind = i;
                break;
            }
        }
        if(ind >= 0)
        {
            //POOL_LOG_D("find idle[%d], size[%d], cap[%d]", ind, m_size, m_capacity);
            return ind;
        }
        if (m_size < m_capacity)
        {
            ind = m_size;
            m_size++;
            m_pool[ind] = std::make_shared<T>();
            //POOL_LOG_D("create new[%d], size[%d], cap[%d]", ind, m_size, m_capacity);
            return ind;
        }
        // fail to find idle item
        POOL_LOG_W("fail to find idle item, size[%d], cap[%d]", m_size, m_capacity);
        return ind;
    }

    void resize(int size)
    {
        POOL_LOG_D("enter, cur size[%d], cap[%d], new size[%d]",
                   m_size, m_capacity, size);
        auto *p = (std::shared_ptr<T> *)(std::malloc(size * sizeof(std::shared_ptr<T>)));
        std::memset(p, 0, size * sizeof(std::shared_ptr<T>));
        std::memcpy(p, m_pool, m_size * sizeof(std::shared_ptr<T>));
        std::shared_ptr<T> *tmp = m_pool;
        m_pool = p;
        std::free(tmp);
        m_capacity = size;
        POOL_LOG_D("finish");
    }

    int32_t m_max;
    int32_t m_size;
    int32_t m_capacity;
    std::mutex m_mutex;

    std::shared_ptr<T> *m_pool;
};

#endif //RENDER_POOL_H
