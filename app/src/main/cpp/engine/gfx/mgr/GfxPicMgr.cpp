#include <GLES3/gl3.h>
#include <mutex>

#include "GfxPicMgr.h"
#include "GfxDef.h"
#include "GfxHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxPicMgr"

#ifdef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace gfx
{
    std::mutex g_mutex{};
    GfxPicMgr *g_mgr{nullptr};

    GfxPicMgr::GfxPicMgr() : m_texMap() {}

    GfxPicMgr::~GfxPicMgr() = default;

    GfxPicMgr *GfxPicMgr::get()
    {
        if (!g_mgr) {
            std::lock_guard<std::mutex> lock(g_mutex);
            if (!g_mgr)
                g_mgr = new GfxPicMgr;
        }
        return g_mgr;
    }

    bool GfxPicMgr::loadTex(const std::string &path, uint32_t &texId, int32_t &width, int32_t &height, int32_t &channel)
    {
        auto itr =  m_texMap.find(path);
        if (itr != m_texMap.end())
        {
            LOG_DEBUG("texture[%s] exists, use cache", path.c_str());
            Texture &tex = itr->second;
            texId = itr->second.m_id;
            channel = itr->second.m_channel;
            width = itr->second.m_width;
            height = itr->second.m_height;
            return true;
        }

        bool ret{false};
        Texture tex;
        unsigned char *data = stbi_load(path.c_str(), &tex.m_width, &tex.m_height, &tex.m_channel, 0);
        if(data)
        {
            GLenum format = GfxHelper::colorFormat(tex.m_channel);

            glGenTextures(1, &tex.m_id);
            glBindTexture(GL_TEXTURE_2D, tex.m_id);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, format, tex.m_width, tex.m_height, 0, format, GL_UNSIGNED_BYTE, data);

            if(GfxHelper::checkGlErr("fail to load texture[%s]", path.c_str()))
            {
                //cache the texture
                width = tex.m_width;
                height = tex.m_height;
                texId = tex.m_id;
                channel = tex.m_channel;
                m_texMap.insert(std::pair<std::string, Texture>(path, tex));
                ret = true;
                LOG_DEBUG("size[%d, %d], channel[%d], path[%s], id[%u]", width, height, channel, path.c_str(), texId);
            }
        }
        else
        {
            LOG_ERR("fail to load img from[%s]", path.c_str());
        }
        stbi_image_free(data);
        return ret;
    }

    void GfxPicMgr::release(const std::string &path, const uint32_t texId)
    {
        auto itr = m_texMap.find(path);
        if (itr != m_texMap.end())
        {
            if (texId == itr->second.m_id)
            {
                LOG_DEBUG("release texture[%s]", path.c_str());
                glDeleteTextures(1, &texId);
            }
            else
            {
                LOG_DEBUG("id[%u] does not match path[%s][%u], delete both",
                          texId, path.c_str(), itr->second.m_id);
                glDeleteTextures(1, &texId);
                glDeleteTextures(1, &(itr->second.m_id));
            }
            m_texMap.erase(itr);
        }
        else
        {
            LOG_DEBUG("fail to find texture[%s] in cache, delete it directly", path.c_str());
            glDeleteTextures(1, &texId);
        }
    }
}

