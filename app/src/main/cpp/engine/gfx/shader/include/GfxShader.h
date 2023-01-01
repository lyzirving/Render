#ifndef RENDER_GFXSHADER_H
#define RENDER_GFXSHADER_H

#include <string>
#include <glm/glm.hpp>

namespace gfx
{
    class GfxShader
    {
    public:
        GfxShader(const char *name, const char *vertPath, const char *fragPath);
        virtual ~GfxShader();

        void init();

        inline bool isInit() { return m_program > 0; }
        inline const std::string &name() { return m_name; }

        void use(bool active);
        void setFloat(const std::string &name, float value) const;
        void setInt(const std::string &name, int value) const;

        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

        void setVec2(const std::string &name, const glm::vec2 &vec2) const;
        void setVec3(const std::string &name, const glm::vec3 &vec3) const;
        void setVec4(const std::string &name, const glm::vec4 &vec4) const;
        void setVec4(const std::string &name, float x0, float x1, float x2, float x3) const;

    protected:
        std::string m_name;
        std::string m_vert, m_frag;
        uint32_t m_program;
    };
}

#endif //RENDER_GFXSHADER_H
