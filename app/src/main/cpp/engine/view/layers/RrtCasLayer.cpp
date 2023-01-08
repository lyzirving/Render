#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "RrtCasLayer.h"
#include "ViewDef.h"

#include "GfxShader.h"
#include "GfxShaderMgr.h"
#include "BVHNode.h"
#include "RrtTriBuf.h"
#include "RrtBVHBuf.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtCasLayer"

using namespace gfx;

namespace view
{
    RrtCasLayer::RrtCasLayer() : Layer(LayerType::CANVAS, LayerOrder::LOW),
                                 m_vao(0) , m_vbo(0), m_ebo(0), m_bgColor(0xffffffff),
                                 m_shader(nullptr), m_BVHBuilder(nullptr),
                                 m_triBuf(new RrtTriBuf), m_BVHBuf(new RrtBVHBuf)
    {
        RrtCasLayer::createItems();
    }

    RrtCasLayer::~RrtCasLayer()
    {
        m_shader.reset();
        m_BVHBuilder.reset();

        m_triBuf.reset();
        m_BVHBuf.reset();
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void RrtCasLayer::createItems()
    {
        initCasMem();

        m_shader = GfxShaderMgr::get()->getShader(ShaderType::CANVAS);

        std::vector<RrtTriangle> triangles{};
        std::vector<RrtBVHNode> nodes{};

        m_BVHBuilder = std::make_shared<BVHBuilder>("StanfordBunny", true, false);
        m_BVHBuilder->buildEncoded(nodes);
        m_BVHBuilder->getTriangles(triangles);

        m_triBuf->addTriangles(triangles);
        m_BVHBuf->addNodes(nodes);
    }

    void RrtCasLayer::drawCall()
    {
        m_shader->setVec4(U_BG_COLOR, R_COMP(m_bgColor), G_COMP(m_bgColor), B_COMP(m_bgColor), A_COMP(m_bgColor));
        m_shader->setVec3(U_EYS_POS, glm::vec3(0.f, 0.f , 5.f));
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void RrtCasLayer::initCasMem()
    {
        gfx::CasVert casVert[4];
        casVert[0].m_pos = glm::vec3(-1.f, 1.f, 0.f);
        casVert[1].m_pos = glm::vec3(-1.f, -1.f, 0.f);
        casVert[2].m_pos = glm::vec3(1.f, 1.f, 0.f);
        casVert[3].m_pos = glm::vec3(1.f, -1.f, 0.f);
        uint32_t casInd[6]{0, 1, 2, 2, 1, 3};

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(CasVert), &casVert[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), &casInd[0], GL_STATIC_DRAW);

        glBindVertexArray(m_vao);
        // bind buffer to vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        // set the vertex attribute pointer, vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CasVert), (void *)nullptr);

        glBindVertexArray(0);
    }

    void RrtCasLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        // if the iteration in fragment shader for each pixel is too large, performance problem will occur.
        // if iteration count is out of driver's limit, gpu will recognize it as dead lock, and crash the application.
        m_shader->use(true);
        m_triBuf->bind(m_shader, 0);
        m_shader->setInt(U_TRI_CNT, /*m_triBuf->triangleCnt()*/20);

        m_BVHBuf->bind(m_shader, 1);
        m_shader->setInt(U_BVH_CNT, m_BVHBuf->nodesCnt());

        drawCall();
        m_shader->use(false);
    }
}

