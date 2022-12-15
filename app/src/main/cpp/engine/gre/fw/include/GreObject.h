#ifndef RENDER_GREOBJECT_H
#define RENDER_GREOBJECT_H

#include <memory>

namespace gre
{
    class GreEventArg
    {
    public:
        GreEventArg();
        virtual ~GreEventArg();

        void* arg0;
    };

    class IEvent
    {
    public:
        IEvent() {}
        virtual ~IEvent() {}

        virtual void fire(std::shared_ptr<GreEventArg> &arg) = 0;
    };

    class GreObject
    {
    public:
        GreObject();

        virtual ~GreObject();
        virtual void slotCallback(std::shared_ptr<GreEventArg>& arg);

    protected:
    };
}

#endif //RENDER_GREOBJECT_H
