#include "GreObject.h"

namespace gre
{
    GreEventArg::GreEventArg() : argInt0(0), argInt1(0),
                                 argObj(nullptr), argData(nullptr) {}

    GreEventArg::~GreEventArg()
    {
        argObj = nullptr;
        argData = nullptr;
    }

    GreEventArg::GreEventArg(GreEventArg &&other)  noexcept
    : argInt0(other.argInt0), argInt1(other.argInt1),
      argObj(std::move(other.argObj)), argData(std::move(other.argData)) {}

    GreEventArg & GreEventArg::operator=(GreEventArg &&other) noexcept
    {
        if(this != &other)
        {
            this->argInt0 = other.argInt0;
            this->argInt1 = other.argInt1;
            this->argObj = other.argObj;
            this->argData = other.argData;
            other.argObj = nullptr;
            other.argData = nullptr;
        }
        return *this;
    }

    GreObject::GreObject() = default;

    GreObject::~GreObject() = default;

    void GreObject::slotCb(PoolEvtArgType &&arg)
    {
        //no implementation in base class
    }
}

