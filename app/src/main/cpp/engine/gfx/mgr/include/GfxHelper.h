#ifndef RENDER_GFXHELPER_H
#define RENDER_GFXHELPER_H

namespace gfx
{
    class GfxHelper
    {
    public:
        static uint32_t createProgram(const char *vert = nullptr, const char *frag = nullptr,
                                      const char *geo = nullptr);
        static uint32_t colorFormat(int component);
        static bool checkGlErr(const char *fmt, ...);

    private:
        static uint32_t createShader(uint32_t type, const char *source);
        static void logShaderInfo(uint32_t shader);

        GfxHelper() {}
        ~GfxHelper() = default;
    };
}

#endif //RENDER_GFXHELPER_H
