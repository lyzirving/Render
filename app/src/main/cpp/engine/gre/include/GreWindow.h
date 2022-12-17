#ifndef RENDER_GREWINDOW_H
#define RENDER_GREWINDOW_H

/**
 * @author  lyzirving
 * @date    2022-12-14
 * @brief   a window that contains scene need to be rendered
**/
#include "GreTimer.h"

namespace gre
{
    class GreWindow : public GreTimer
    {
    public:
        GreWindow(GreContextId id);
        virtual ~GreWindow();

    protected:
        virtual void slotCb(PoolEvtArgType &&arg) override;

    private:
        GreContextId m_id;
    };
}

#endif //RENDER_GREWINDOW_H
