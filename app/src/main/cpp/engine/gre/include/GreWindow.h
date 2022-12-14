#ifndef RENDER_GREWINDOW_H
#define RENDER_GREWINDOW_H

/**
 * @author  lyzirving
 * @date    2022-12-14
 * @brief   a window that contains scene need to be rendered
**/
#include "GreDef.h"

namespace gre
{
    class GreWindow
    {
    public:
        GreWindow(GreContextId id);
        virtual ~GreWindow();

    private:
        GreContextId m_id;
    };
}

#endif //RENDER_GREWINDOW_H
