#include <mutex>

#include "GfxShaderMgr.h"
#include "GfxShader.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxShaderMgr"

namespace gfx
{
    static std::mutex g_mutex{};
    static GfxShaderMgr *g_mgr{nullptr};

    static const char *STR_NONE   = "none";
    static const char *STR_OBJ    = "obj";
    static const char *STR_SPHERE = "sphere";
    static const char *STR_CANVAS = "cas";

    GfxShaderMgr * GfxShaderMgr::get()
    {
        if (!g_mgr) {
            std::lock_guard<std::mutex> lock(g_mutex);
            if (!g_mgr)
                g_mgr = new GfxShaderMgr;
        }
        return g_mgr;
    }

    GfxShaderMgr::GfxShaderMgr() : m_shaders() {}

    GfxShaderMgr::~GfxShaderMgr() { release(); }

    const std::shared_ptr<GfxShader> & GfxShaderMgr::getShader(ShaderType type)
    {
        std::string key(type2Str(type));
        auto itr = m_shaders.find(key);
        if (itr != m_shaders.end())
        {
            const std::shared_ptr<GfxShader> &shader = itr->second;
            return shader;
        }
        else
        {
            LOG_ERR("fail to find shader[%u][%s]", type, type2Str(type));
            return std::shared_ptr<GfxShader>(nullptr);
        }
    }

    void GfxShaderMgr::init()
    {
        std::string key = STR_OBJ;
        std::shared_ptr<GfxShader> objShader = std::make_shared<GfxShader>(STR_OBJ, STR_OBJ, STR_OBJ);
        m_shaders.insert(std::pair<std::string, std::shared_ptr<GfxShader>>(key, objShader));

        key = STR_SPHERE;
        std::shared_ptr<GfxShader> sphereShader = std::make_shared<GfxShader>(STR_SPHERE, STR_OBJ, STR_SPHERE);
        m_shaders.insert(std::pair<std::string, std::shared_ptr<GfxShader>>(key, sphereShader));

        key = STR_CANVAS;
        std::shared_ptr<GfxShader> casShader = std::make_shared<GfxShader>(STR_CANVAS, STR_CANVAS, STR_CANVAS);
        m_shaders.insert(std::pair<std::string, std::shared_ptr<GfxShader>>(key, casShader));
    }

    void GfxShaderMgr::release()
    {
        auto itr = m_shaders.begin();
        while(itr != m_shaders.end())
        {
            itr->second.reset();
            itr = m_shaders.erase(itr);
        }
    }

    const char * GfxShaderMgr::type2Str(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::OBJ:
            {
                return STR_OBJ;
            }
            case ShaderType::SPHERE:
            {
                return STR_SPHERE;
            }
            case ShaderType::CANVAS:
            {
                return STR_CANVAS;
            }
            default:
            {
                return STR_NONE;
            }
        }
    }

}

