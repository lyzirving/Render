#include "GreObject.h"

namespace gre
{
    GreEventArg::GreEventArg() : arg0(nullptr) {}

    GreEventArg::~GreEventArg()
    {
        arg0 = nullptr;
    }

    GreObject::GreObject() = default;

    GreObject::~GreObject() = default;

    void GreObject::slotCb(PoolEvtArgType &&arg)
    {
        //no implementation in base class
    }
}

