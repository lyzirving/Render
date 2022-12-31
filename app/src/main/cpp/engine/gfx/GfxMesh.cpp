#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cmath>

#include "GfxMesh.h"
#include "Shader.h"
#include "GfxHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxMesh"

namespace gfx
{
    GfxMesh::GfxMesh() : VideoMem(),
                         m_vertex(), m_indices(), m_textures(),
                         m_material(new Material),
                         m_drawMode(DrawMode::MODE_TRIANGLE),
                         m_initialized(false)
    {
    }

    GfxMesh::GfxMesh(const char *name) : VideoMem(name),
                                         m_vertex(), m_indices(), m_textures(),
                                         m_material(new Material),
                                         m_drawMode(DrawMode::MODE_TRIANGLE),
                                         m_initialized(false)
    {
    }

    GfxMesh::GfxMesh(GfxMesh &&other) noexcept
    {
        if (this != &other) {
            this->m_vertex = std::move(other.m_vertex);
            this->m_indices = std::move(other.m_indices);
            this->m_textures = std::move(other.m_textures);
            this->m_material = std::move(other.m_material);
            this->m_vao = other.m_vao;
            this->m_vbo = other.m_vbo;
            this->m_ebo = other.m_ebo;
            this->m_name = std::move(other.m_name);
            this->m_drawMode = other.m_drawMode;
            this->m_initialized = other.m_initialized;
            other.m_initialized = false;
            other.m_vao = 0;
            other.m_vbo = 0;
            other.m_ebo = 0;
        }
    }

    GfxMesh &GfxMesh::operator=(GfxMesh &&other) noexcept
    {
        if (this != &other) {
            this->m_vertex = std::move(other.m_vertex);
            this->m_indices = std::move(other.m_indices);
            this->m_textures = std::move(other.m_textures);
            this->m_material = std::move(other.m_material);
            this->m_vao = other.m_vao;
            this->m_vbo = other.m_vbo;
            this->m_ebo = other.m_ebo;
            this->m_name = std::move(other.m_name);
            this->m_drawMode = other.m_drawMode;
            this->m_initialized = other.m_initialized;
            other.m_initialized = false;
            other.m_vao = 0;
            other.m_vbo = 0;
            other.m_ebo = 0;
        }
        return *this;
    }

    GfxMesh::~GfxMesh()
    {
        release();
    }

    void GfxMesh::bind(bool force)
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

    void GfxMesh::draw(const std::shared_ptr<Shader> &shader)
    {
        for (int32_t i = 0; i < m_textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i]->getId());
            //todo bind texture to shader
        }
        glBindVertexArray(m_vao);
        glDrawElements(getGlDrawMode(), m_indices.size(), GL_UNSIGNED_INT, nullptr);
        GfxHelper::checkGlErr("mesh[%s] draw err", m_name.c_str());
        glBindVertexArray(0);
    }

    uint32_t GfxMesh::getGlDrawMode()
    {
        switch (m_drawMode)
        {
            case DrawMode::MODE_LINE:
                return GL_LINES;
            case DrawMode::MODE_POINT:
                return GL_POINTS;
            case DrawMode::MODE_TRIANGLE:
            default:
                return GL_TRIANGLES;
        }
    }

    void GfxMesh::release()
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

