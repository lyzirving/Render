#ifndef RENDER_GFXSHADERMGR_H
#define RENDER_GFXSHADERMGR_H

#include <map>
#include <memory>

#include "GfxDef.h"

namespace gfx
{
    class GfxShader;

    class GfxShaderMgr
    {
    public:
        static GfxShaderMgr* get();

        void init();
        void release();

        const std::shared_ptr<GfxShader> &getShader(ShaderType type);
    private:
        GfxShaderMgr();
        ~GfxShaderMgr();

        static const char *type2Str(ShaderType type);

        std::map<std::string, std::shared_ptr<GfxShader>> m_shaders;
    };
}

#endif //RENDER_GFXSHADERMGR_H
