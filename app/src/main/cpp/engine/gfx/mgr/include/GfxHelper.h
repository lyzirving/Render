#ifndef RENDER_GFXHELPER_H
#define RENDER_GFXHELPER_H

namespace gfx
{
    class GfxHelper
    {
    public:
        static uint32_t colorFormat(int component);
        static bool checkGlErr(const char *fmt, ...);

    private:
        GfxHelper() {}
        ~GfxHelper() = default;
    };
}

#endif //RENDER_GFXHELPER_H
