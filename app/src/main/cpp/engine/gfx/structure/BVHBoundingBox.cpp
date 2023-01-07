#include <limits>
#include <GLES3/gl3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "BVHBoundingBox.h"
#include "BVHNode.h"
#include "GfxShaderMgr.h"
#include "GfxShader.h"

#include "ViewConv.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHBoundingBox"

using namespace view;

namespace gfx
{
    BVHBoundingBox::BVHBoundingBox() : GfxMesh(), m_max(FLT_MIN), m_min(FLT_MAX), m_modelMt(1.f)
    {
        m_drawMode = DrawMode::MODE_LINE;
    }

    BVHBoundingBox::BVHBoundingBox(const char *name) : GfxMesh(name), m_max(FLT_MIN), m_min(FLT_MAX),
                                                       m_modelMt(1.f)
    {
        m_drawMode = DrawMode::MODE_LINE;
    }

    BVHBoundingBox::~BVHBoundingBox() = default;

    void BVHBoundingBox::adjModel()
    {
        glm::mat4 mtTrans{1.f}, mtScale{1.f};

        glm::vec3 center = (m_max + m_min) * 0.5f;
        mtTrans = glm::translate(mtTrans, -center);

        glm::vec3 interval = m_max - m_min;
        float scale = 2.f / std::max(std::max(interval.x, interval.y), interval.z);
        mtScale = glm::scale(mtScale, glm::vec3(scale));

        m_modelMt = mtScale * mtTrans;
    }

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

    void BVHBoundingBox::draw(const std::shared_ptr<view::ViewConv> &conv)
    {
        const glm::mat4 &viewMt = conv->getViewMat();
        const glm::mat4 &prjMt = conv->getProjectMat();

        const std::shared_ptr<gfx::GfxShader> &shader = GfxShaderMgr::get()->getShader(ShaderType::COLOR_OBJ);
        shader->use(true);
        shader->setMat4(U_MT_VIEW, viewMt);
        shader->setMat4(U_MT_PROJ, prjMt);
        shader->setMat4(U_MT_MODEL, m_modelMt);
        shader->setVec4(U_COLOR, 0.f, 0.f, 1.f, 0.8f);
        glLineWidth(3.f);

        glBindVertexArray(m_vao);
        glDrawArrays(getGlDrawMode(), 0, m_vertex.size());
        glBindVertexArray(0);

        shader->use(false);
    }

    void BVHBoundingBox::getBVHBound(const std::shared_ptr<gfx::BVHNode> &node)
    {
        if (!node)
        {
            LOG_ERR("root node is null");
            return;
        }
        itrBVH(node);
        adjModel();
        LOG_DEBUG("box vertex count[%lu]", m_vertex.size());
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
        const glm::vec3& aa = node->m_AA;
        const glm::vec3& bb = node->m_BB;
        Vertex pt0{}, pt1{};

        // edge perpendicular to plane x-y
        // right-top
        pt0.m_pos = glm::vec3(bb.x, bb.y, bb.z);
        pt1.m_pos = glm::vec3(bb.x, bb.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // right-bottom
        pt0.m_pos = glm::vec3(bb.x, aa.y, bb.z);
        pt1.m_pos = glm::vec3(bb.x, aa.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // left-bottom
        pt0.m_pos = glm::vec3(aa.x, aa.y, bb.z);
        pt1.m_pos = glm::vec3(aa.x, aa.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // left-top
        pt0.m_pos = glm::vec3(aa.x, bb.y, bb.z);
        pt1.m_pos = glm::vec3(aa.x, bb.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // edge perpendicular to plane y-z
        // front-top
        pt0.m_pos = glm::vec3(aa.x, bb.y, bb.z);
        pt1.m_pos = glm::vec3(bb.x, bb.y, bb.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // front-bottom
        pt0.m_pos = glm::vec3(aa.x, aa.y, bb.z);
        pt1.m_pos = glm::vec3(bb.x, aa.y, bb.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // back-bottom
        pt0.m_pos = glm::vec3(aa.x, aa.y, aa.z);
        pt1.m_pos = glm::vec3(bb.x, aa.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // back-top
        pt0.m_pos = glm::vec3(aa.x, bb.y, aa.z);
        pt1.m_pos = glm::vec3(bb.x, bb.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // edge perpendicular to plane x-z
        // left-front
        pt0.m_pos = glm::vec3(aa.x, bb.y, bb.z);
        pt1.m_pos = glm::vec3(aa.x, aa.y, bb.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // left-back
        pt0.m_pos = glm::vec3(aa.x, bb.y, aa.z);
        pt1.m_pos = glm::vec3(aa.x, aa.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // right-back
        pt0.m_pos = glm::vec3(bb.x, bb.y, aa.z);
        pt1.m_pos = glm::vec3(bb.x, aa.y, aa.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);

        // right-front
        pt0.m_pos = glm::vec3(bb.x, bb.y, bb.z);
        pt1.m_pos = glm::vec3(bb.x, aa.y, bb.z);
        m_max = glm::max(m_max, pt0.m_pos);
        m_max = glm::max(m_max, pt1.m_pos);
        m_min = glm::min(m_min, pt0.m_pos);
        m_min = glm::min(m_min, pt1.m_pos);
        m_vertex.push_back(pt0);
        m_vertex.push_back(pt1);
    }

}

