#include "GreObject.h"

namespace gre
{
    GreEventArg::GreEventArg() : argPtr(nullptr) {}

    GreEventArg::~GreEventArg()
    {
        argPtr = nullptr;
    }

    GreObject::GreObject() = default;

    GreObject::~GreObject() = default;

    void GreObject::slotCb(PoolEvtArgType &&arg)
    {
        //no implementation in base class
    }
}

