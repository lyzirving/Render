#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cmath>

#include "Mesh.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace gfx
{
    Mesh::Mesh() : VideoMem(),
                   m_vertex(), m_indices(), m_textures(),
                   m_material(nullptr),
                   m_drawMode(DrawMode::MODE_TRIANGLE), m_initialized(false)
    {
    }

    Mesh::Mesh(Mesh &&other) noexcept
    {
        if (this != &other) {
            this->m_vertex = std::move(other.m_vertex);
            this->m_indices = std::move(other.m_indices);
            this->m_textures = std::move(other.m_textures);
            this->m_material = std::move(other.m_material);
            this->m_vao = other.m_vao;
            this->m_vbo = other.m_vbo;
            this->m_ebo = other.m_ebo;
            this->m_drawMode = other.m_drawMode;
            this->m_initialized = other.m_initialized;
            other.m_initialized = false;
            other.m_vao = 0;
            other.m_vbo = 0;
            other.m_ebo = 0;
        }
    }

    Mesh &Mesh::operator=(Mesh &&other) noexcept
    {
        if (this != &other) {
            this->m_vertex = std::move(other.m_vertex);
            this->m_indices = std::move(other.m_indices);
            this->m_textures = std::move(other.m_textures);
            this->m_material = std::move(other.m_material);
            this->m_vao = other.m_vao;
            this->m_vbo = other.m_vbo;
            this->m_ebo = other.m_ebo;
            this->m_drawMode = other.m_drawMode;
            this->m_initialized = other.m_initialized;
            other.m_initialized = false;
            other.m_vao = 0;
            other.m_vbo = 0;
            other.m_ebo = 0;
        }
        return *this;
    }

    Mesh::~Mesh()
    {

    }

    void Mesh::setupMesh(bool force)
    {

    }

    void Mesh::draw(const std::shared_ptr<Shader> &shader)
    {

    }

    void Mesh::release()
    {

    }

}

