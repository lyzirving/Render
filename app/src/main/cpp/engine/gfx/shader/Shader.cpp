#include <iostream>
#include <fstream>
#include <sstream>
#include <GLES3/gl3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "AssetsMgr.h"
#include "GfxHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace gfx
{
    Shader::Shader(const char *name, const char *vertPath, const char *fragPath)
    : m_name(name), m_vert(), m_frag(), m_program(0)
    {
        std::ifstream vertFile, fragFile;
        std::stringstream vertStream, fragStream;

        vertFile.open(AssetsMgr::getVert(vertPath));
        if(!vertFile.is_open())
        {
            LOG_ERR("fail to open vertex shader[%s]", vertPath);
            assert(0);
        }
        fragFile.open(AssetsMgr::getFrag(fragPath));
        if(!fragFile.is_open())
        {
            LOG_ERR("fail to open fragment shader[%s]", fragPath);
            assert(0);
        }

        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        m_vert = vertStream.str();
        m_frag = fragStream.str();
    }

    Shader::~Shader() = default;

    void Shader::init()
    {
        if (m_vert.empty() || m_frag.empty())
        {
            LOG_ERR("shader[%s] is empty\n[%s] \n[%s]", m_name.c_str(), m_vert.c_str(), m_frag.c_str());
            assert(0);
        }
        if (isInit())
        {
            return;
        }
        m_program = GfxHelper::createProgram(m_vert.c_str(), m_frag.c_str());
        if(m_program == 0)
        {
            LOG_ERR("fail to create program for shader[%s]\n[%s] \n[%s]",
                    m_name.c_str(), m_vert.c_str(), m_frag.c_str());
            assert(0);
        }
    }

    void Shader::use(bool active)
    {
        if(isInit()) glUseProgram(active ? m_program : 0);
    }

    void Shader::setFloat(const std::string &name, float value) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniform1f(ind, value);
    }

    void Shader::setInt(const std::string &name, int value) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniform1i(ind, value);
    }

    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniformMatrix3fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &vec2) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniform2f(ind, vec2.x, vec2.y);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &vec3) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniform3f(ind, vec3.x, vec3.y, vec3.z);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &vec4) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniform4f(ind, vec4.x, vec4.y, vec4.z, vec4.w);
    }

    void Shader::setVec4(const std::string &name, float x0, float x1, float x2, float x3) const
    {
        GLint ind = glGetUniformLocation(m_program, name.c_str());
        glUniform4f(ind, x0, x1, x2, x3);
    }
}

