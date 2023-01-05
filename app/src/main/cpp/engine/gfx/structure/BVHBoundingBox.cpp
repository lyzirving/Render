#include "BVHBoundingBox.h"
#include "BVHNode.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHBoundingBox"

namespace gfx
{
    BVHBoundingBox::BVHBoundingBox() : GfxMesh()
    {
        m_drawMode = DrawMode::MODE_LINE;
    }

    BVHBoundingBox::BVHBoundingBox(const char *name) : GfxMesh(name)
    {
        m_drawMode = DrawMode::MODE_LINE;
    }

    BVHBoundingBox::~BVHBoundingBox() = default;

    void BVHBoundingBox::bind(bool force)
    {
        if(force)
            goto setup;

        if(m_initialized)
            return;

        if(m_vertex.empty())
            return;

        setup:
        createMem();

        // load vertex data into array buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(Vertex), &m_vertex[0], GL_STATIC_DRAW);
        // **********finish create buffer **********

        glBindVertexArray(m_vao);
        // bind buffer to vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        // set the vertex attribute pointer, vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);

        glBindVertexArray(0);

        m_initialized = true;
    }

    void BVHBoundingBox::createMem()
    {
        if(m_vao == 0) glGenVertexArrays(1, &m_vao);
        if(m_vbo == 0) glGenBuffers(1, &m_vbo);
        LOG_DEBUG("mem[%s] create vao[%u], vbo[%u]", m_name.c_str(), m_vao, m_vbo);
    }

    void BVHBoundingBox::draw(const std::shared_ptr<gfx::GfxShader> &shader)
    {
        glBindVertexArray(m_vao);
        glDrawArrays(getGlDrawMode(), 0, m_vertex.size());
        glBindVertexArray(0);
    }

    void BVHBoundingBox::getBVHBound(const std::shared_ptr<gfx::BVHNode> &node)
    {
        if (!node)
        {
            LOG_ERR("root node is null");
            return;
        }
        itrBVH(node);
        bind(true);
    }

    void BVHBoundingBox::itrBVH(const std::shared_ptr<gfx::BVHNode> &node)
    {
        if(node)
        {
            parseBVH(node);
            itrBVH(node->m_left);
            itrBVH(node->m_right);
        }
    }

    void BVHBoundingBox::parseBVH(const std::shared_ptr<gfx::BVHNode> &node)
    {
        const glm::vec3& min = node->m_AA;
        const glm::vec3& max = node->m_BB;

        Vertex pt0{}, pt1{};

        // edge perpendicular to plane x-y
        // right-top
        pt0.m_pos = glm::vec3(max.x, max.y, max.z);
        pt1.m_pos = glm::vec3(max.x, max.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // right-bottom
        pt0.m_pos = glm::vec3(max.x, min.y, max.z);
        pt1.m_pos = glm::vec3(max.x, min.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // left-bottom
        pt0.m_pos = glm::vec3(min.x, min.y, max.z);
        pt1.m_pos = glm::vec3(min.x, min.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // left-top
        pt0.m_pos = glm::vec3(min.x, max.y, max.z);
        pt1.m_pos = glm::vec3(min.x, max.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // edge perpendicular to plane y-z
        // front-top
        pt0.m_pos = glm::vec3(min.x, max.y, max.z);
        pt1.m_pos = glm::vec3(max.x, max.y, max.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // front-bottom
        pt0.m_pos = glm::vec3(min.x, min.y, max.z);
        pt1.m_pos = glm::vec3(max.x, min.y, max.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // back-bottom
        pt0.m_pos = glm::vec3(min.x, min.y, min.z);
        pt1.m_pos = glm::vec3(max.x, min.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // back-top
        pt0.m_pos = glm::vec3(min.x, max.y, min.z);
        pt1.m_pos = glm::vec3(max.x, max.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // edge perpendicular to plane x-z
        // left-front
        pt0.m_pos = glm::vec3(min.x, max.y, max.z);
        pt1.m_pos = glm::vec3(min.x, min.y, max.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // left-back
        pt0.m_pos = glm::vec3(min.x, max.y, min.z);
        pt1.m_pos = glm::vec3(min.x, min.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // right-back
        pt0.m_pos = glm::vec3(max.x, max.y, min.z);
        pt1.m_pos = glm::vec3(max.x, min.y, min.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // right-front
        pt0.m_pos = glm::vec3(max.x, max.y, max.z);
        pt1.m_pos = glm::vec3(max.x, min.y, max.z);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);
    }

}

