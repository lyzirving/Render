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
                   m_material(new Material),
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
        release();
    }

    void Mesh::bind(bool force)
    {
        if(force)
            goto setup;

        if(m_initialized)
            return;

        if(m_vertex.empty() || m_indices.empty())
            return;

     setup:
        createMem();

        // load vertex data into array buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(Vertex), &m_vertex[0], GL_STATIC_DRAW);
        // load indices data into element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
        // **********finish create buffer **********

        glBindVertexArray(m_vao);
        // bind buffer to vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        // set the vertex attribute pointer, vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);
        // vertex normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_normal));
        // texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_tex));

        glBindVertexArray(0);

        m_initialized = true;
    }

    void Mesh::draw(const std::shared_ptr<Shader> &shader)
    {

    }

    void Mesh::release()
    {
        if (!m_vertex.empty())
        {
            std::vector<Vertex> tmp;
            m_vertex.swap(tmp);
        }
        if (!m_indices.empty())
        {
            std::vector<uint32_t> tmp;
            m_indices.swap(tmp);
        }
        if (!m_textures.empty())
        {
            auto itr = m_textures.begin();
            while (itr != m_textures.end())
            {
                (*itr).reset();
                itr = m_textures.erase(itr);
            }
            std::vector<std::shared_ptr<Texture>> tmp;
            m_textures.swap(tmp);
        }
        m_material.reset();
        destroyMem();
    }

}

