#include "Sphere.h"
#include "GfxMesh.h"
#include "GfxShaderMgr.h"
#include "GfxShader.h"

#include "ViewConv.h"
#include "ViewDef.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Sphere"

using namespace gfx;

namespace view
{
    Sphere::Sphere(const char *name, uint32_t horSegCnt, uint32_t verSegCnt, float radius)
    : Model(), m_horSegCnt(horSegCnt), m_verSegCnt(verSegCnt), m_radius(radius),
      m_color(0x0000ffff)
    {
        m_name = name;
        build();
        calcCentral();
    }

    Sphere::~Sphere() = default;

    void Sphere::build()
    {
        std::shared_ptr<GfxMesh> mesh = std::make_shared<GfxMesh>(m_name.c_str());
        std::vector<Vertex> &vertexList = mesh->getVertex();
        std::vector<uint32_t> &indList = mesh->getIndices();

        // notice on y axis, the iteration count is m_verSegCnt + 1,
        // on x axis, the iteration count is m_horSegCnt
        for(int y = 0; y <= m_verSegCnt; y++)
        {
            for(int x = 0; x < m_horSegCnt; x++)
            {
                // build vertex for sphere
                if((y == 0 && x == 0) || (y == m_verSegCnt && x == 0) || (y != 0 && y != m_verSegCnt))
                {
                    Vertex vertex{};
                    // theta is the angle between vector projected on x-z plane and +z
                    // phi is the angle between vector and +y
                    float theta = float(x) / float(m_horSegCnt);
                    float phi = float(y) / float(m_verSegCnt);
                    auto xPos = float(std::sin(phi * M_PI) * std::sin(theta * 2 * M_PI));
                    auto yPos = float(std::cos(phi * M_PI));
                    auto zPos = float(std::sin(phi * M_PI) * std::cos(theta * 2 * M_PI));

                    glm::vec3 vec = glm::normalize(glm::vec3(xPos, yPos, zPos));
                    vertex.m_pos = m_radius * vec;
                    vertex.m_normal = vec;
                    updateMinMax(vertex.m_pos);
                    vertexList.push_back(vertex);
                }

                // add indices for sphere
                bool pickFirst = (x >= (m_horSegCnt - 1));
                if(y == 1)
                {
                    // add triangles for rendering
                    uint32_t ind = vertInd(x, y);
                    indList.push_back(0);
                    indList.push_back(ind);
                    indList.push_back(pickFirst ? vertInd(0, y) : (ind + 1));
                }
                else if(y == m_verSegCnt)
                {
                    // add triangles for rendering
                    uint32_t endInd = allVertNum() - 1;
                    indList.push_back(vertInd(x, y - 1));
                    indList.push_back(endInd);
                    indList.push_back(vertInd(pickFirst ? 0 : x + 1, y - 1));
                }
                else if(y != 0)
                {
                    // add rectangles for rendering
                    if (pickFirst)
                    {
                        uint32_t lastInd = vertInd(x, y - 1);
                        uint32_t lastFirstInd = vertInd(0, y - 1);
                        uint32_t curInd = vertInd(x, y);
                        uint32_t curFirstInd = vertInd(0, y);

                        indList.push_back(lastInd);
                        indList.push_back(curInd);
                        indList.push_back(lastFirstInd);

                        indList.push_back(lastFirstInd);
                        indList.push_back(curInd);
                        indList.push_back(curFirstInd);
                    }
                    else
                    {
                        uint32_t lastInd = vertInd(x, y - 1);
                        uint32_t curInd = vertInd(x, y);

                        indList.push_back(lastInd);
                        indList.push_back(curInd);
                        indList.push_back(lastInd + 1);

                        indList.push_back(lastInd + 1);
                        indList.push_back(curInd);
                        indList.push_back(curInd + 1);
                    }
                }
            }
        }

        mesh->bind(true);
        calcCentral();
        m_mesh.push_back(std::move(mesh));
    }

    uint32_t Sphere::vertInd(uint32_t x, uint32_t y)
    {
        if(x == 0 && y == 0)
            return 0;
        else if(x == 0 && y == m_verSegCnt)
            return allVertNum() - 1;
        else if(y != 0 && y != m_verSegCnt)
            return (1 + (y - 1) * m_horSegCnt + x);

        LOG_ERR("out of bounds err for ind[%u, %u], segments[%u, %u], return 0", x, y, m_horSegCnt, m_verSegCnt);
        return 0;
    }

    void Sphere::draw(const std::shared_ptr<ViewConv> &conv)
    {
        const glm::mat4 &viewMt = conv->getViewMat();
        const glm::mat4 &prjMt = conv->getProjectMat();

        const std::shared_ptr<GfxShader> &shader = GfxShaderMgr::get()->getShader(ShaderType::SPHERE);
        shader->use(true);
        shader->setMat4(U_MT_VIEW, viewMt);
        shader->setMat4(U_MT_PROJ, prjMt);
        shader->setMat4(U_MT_MODEL, m_mtModel * m_mtCenter);
        shader->setVec4(U_COLOR, glm::vec4(RED_CHANNEL(m_color), GREEN_CHANNEL(m_color),
                                           BLUE_CHANNEL(m_color), ALPHA_CHANNEL(m_color)));

        for(auto &item : m_mesh)
        {
            item->draw(shader);
        }
        shader->use(false);
    }
}

