
#ifndef RENDER_GFXDEF_H
#define RENDER_GFXDEF_H

#include <string>
#include <glm/glm.hpp>

#include "GfxMem.h"

namespace gfx
{
    enum ShaderType : uint8_t
    {
        OBJ,
        SHADER_CNT
    };

    enum DrawMode : uint8_t
    {
        MODE_TRIANGLE,
        MODE_LINE,
        MODE_POINT
    };

    enum TextureType : uint8_t
    {
        DIFFUSE,
        SPECULAR
    };

    struct Vertex
    {
        glm::vec3 m_pos;
        glm::vec3 m_normal;
        glm::vec2 m_tex;

        Vertex() : m_pos(0.f), m_normal(0.f), m_tex(0.f) {}
    };

    class Texture : public PicMem
    {
    public:
        Texture() : PicMem() {}
        Texture(const char *path, TextureType type) : PicMem(path), m_type(type) {}
        virtual ~Texture() = default;

    protected:
        TextureType m_type;
    };

    class Material
    {
    public:
        Material() : m_ka(0.f), m_kd(0.f), m_ks(0.f) {}
        virtual ~Material() = default;

        inline void setKa(const glm::vec3 & ka) { m_ka = ka; }
        inline void setKa(float x, float y, float z) { m_ka.x = x; m_ka.y = y; m_ka.z = z; }
        inline void setKd(const glm::vec3 & kd) { m_kd = kd; }
        inline void setKd(float x, float y, float z) { m_kd.x = x; m_kd.y = y; m_kd.z = z; }
        inline void setKs(const glm::vec3 & ks) { m_ks = ks; }
        inline void setKs(float x, float y, float z) { m_ks.x = x; m_ks.y = y; m_ks.z = z; }
        inline const glm::vec3 &getKa() { return m_ka; }
        inline const glm::vec3 &getKd() { return m_kd; }
        inline const glm::vec3 &getKs() { return m_ks; }

    protected:
        glm::vec3 m_ka, m_kd, m_ks;
    };
}

#endif //RENDER_GFXDEF_H
