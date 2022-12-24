#ifndef RENDER_GREWINDOW_H
#define RENDER_GREWINDOW_H

/**
 * @author  lyzirving
 * @date    2022-12-14
 * @brief   a window that contains scene need to be rendered
**/
#include "GreTimer.h"

struct ANativeWindow;

namespace gfx
{
    class GfxEglCore;
}

namespace gre
{
    class GreWindow : public GreTimer
    {
    public:
        GreWindow(GreContextId id);
        virtual ~GreWindow();

        virtual void slotCb(const PoolEvtArg &arg) override;

        bool attachSurface(ANativeWindow *surface);
        void release();

    private:
        void printFps();

        GreContextId m_id;
        uint64_t m_totalFrame;
        int64_t m_lastRecTimeMs;
        uint8_t m_fps;

        std::shared_ptr<gfx::GfxEglCore> m_egl;
    };
}

#endif //RENDER_GREWINDOW_H
