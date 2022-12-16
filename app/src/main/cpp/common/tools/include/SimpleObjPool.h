#ifndef RENDER_SIMPLEOBJPOOL_H
#define RENDER_SIMPLEOBJPOOL_H

#include <memory>
#include <list>

template <class T>
class SimpleObjPool
{
public:
    SimpleObjPool() : m_pool() {};
    ~SimpleObjPool() = default;

    void push()
    {

    }

private:
    struct SimpleObjDeleter
    {
        void operator() (T *ptr)
        {

        }
    };

    using Type = std::unique_ptr<T, SimpleObjDeleter>;

    std::list<Type> m_pool;
};

#endif //RENDER_SIMPLEOBJPOOL_H
