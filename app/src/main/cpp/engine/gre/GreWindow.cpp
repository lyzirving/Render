#include "GreWindow.h"

namespace gre
{
    GreWindow::GreWindow(GreContextId id)
    : GreTimer(GreEventId::REFRESH, 1000 / 60, GrePriority::TOP),
      m_id(id)
    {
    }

    GreWindow::~GreWindow() = default;

    void GreWindow::slotCb(PoolEvtArgType &&arg)
    {

    }

}