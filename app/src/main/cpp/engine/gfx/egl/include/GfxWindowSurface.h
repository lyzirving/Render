#ifndef RENDER_GFXWINDOWSURFACE_H
#define RENDER_GFXWINDOWSURFACE_H

#include <memory>

#include "GfxEglCore.h"

struct ANativeWindow;

namespace gfx
{
    class GfxWindowSurface
    {
    public:
        GfxWindowSurface(const std::shared_ptr<GfxEglCore> &eglCore, ANativeWindow *window, uint8_t id);
        virtual ~GfxWindowSurface();

        bool isPrepared();
        bool makeCurrent();
        bool prepare();
        void release();
        bool swapBuffer();

        inline EGLSurface getSurface() { return m_surface; }
        inline int32_t getWidth() { return m_width; }
        inline int32_t getHeight() { return m_height; }

    private:
        std::shared_ptr<GfxEglCore> m_eglCore;
        ANativeWindow *m_window;
        EGLSurface m_surface;

        uint8_t m_id;
        int32_t m_width, m_height;
    };
}

#endif //RENDER_GFXWINDOWSURFACE_H
