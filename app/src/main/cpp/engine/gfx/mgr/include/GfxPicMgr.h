#ifndef RENDER_GFXPICMGR_H
#define RENDER_GFXPICMGR_H

#include <string>
#include <unordered_map>

namespace gfx
{
    class Texture;

    class GfxPicMgr
    {
    public:
        static GfxPicMgr *get();

        bool loadTex(const std::string &path, uint32_t &texId, int32_t &width, int32_t &height, int32_t &channel);
        void release(const std::string &path, const uint32_t texId);

    private:
        GfxPicMgr();
        ~GfxPicMgr();

        std::unordered_map<std::string, Texture> m_texMap;
    };
}

#endif //RENDER_GFXPICMGR_H
