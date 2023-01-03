#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "RtxCasLayer.h"
#include "ViewDef.h"
#include "RtxTriBuf.h"

#include "GfxShader.h"
#include "GfxShaderMgr.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RtxCasLayer"

using namespace gfx;

namespace view
{
    RtxCasLayer::RtxCasLayer() : Layer(LayerType::CANVAS, LayerOrder::LOW),
                                         m_vao(0) , m_vbo(0), m_ebo(0), m_bgColor(0xffffffff),
                                         m_shader(nullptr), m_triBuf(new RtxTriBuf)
    {
        m_canvas[0].m_pos = glm::vec3(-1.f, 1.f, 0.f);
        m_canvas[1].m_pos = glm::vec3(-1.f, -1.f, 0.f);
        m_canvas[2].m_pos = glm::vec3(1.f, 1.f, 0.f);
        m_canvas[3].m_pos = glm::vec3(1.f, -1.f, 0.f);

        initVideoMem();

        m_shader = GfxShaderMgr::get()->getShader(ShaderType::CANVAS);

        RtxTriangle tri{};
        tri.p0 = glm::vec3(0.f, 0.5f, 0.f);
        tri.p1 = glm::vec3(-0.5f, 0.f, 0.f);
        tri.p2 = glm::vec3(0.5f, -0.5f, 0.f);
        tri.color = glm::vec3(1.f, 0.f, 0.f);

        std::vector<RtxTriangle> triangles{};
        triangles.push_back(tri);

        m_triBuf->addTriangles(triangles);
    }

    RtxCasLayer::~RtxCasLayer()
    {
        m_triBuf.reset();
        m_shader.reset();
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void RtxCasLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        m_shader->use(true);
        m_triBuf->bind(m_shader, 0);
        m_shader->setInt(U_TRI_CNT, m_triBuf->triangleCnt());
        drawCall();
        m_shader->use(false);
    }

    void RtxCasLayer::createItems() {}

    void RtxCasLayer::drawCall()
    {
        m_shader->setVec4(U_BG_COLOR, R_COMP(m_bgColor), G_COMP(m_bgColor), B_COMP(m_bgColor), A_COMP(m_bgColor));
        m_shader->setVec3(U_EYS_POS, glm::vec3(0.f, 0.f , 5.f));
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void RtxCasLayer::initVideoMem()
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(CasVert), &m_canvas[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), &m_casInd[0], GL_STATIC_DRAW);

        glBindVertexArray(m_vao);
        // bind buffer to vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        // set the vertex attribute pointer, vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CasVert), (void *)nullptr);

        glBindVertexArray(0);
    }
}

