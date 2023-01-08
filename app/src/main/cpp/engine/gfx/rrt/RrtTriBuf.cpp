#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include "RrtTriBuf.h"

#include "GfxShader.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtTriBuf"

namespace gfx
{
    RrtTriBuf::RrtTriBuf() : RrtBuffer(), m_triangles()
    {
        m_slot = RRT_SLOT_TRI;
    }

    RrtTriBuf::~RrtTriBuf()
    {
        std::vector<RrtTriangle>().swap(m_triangles);
    }

    void RrtTriBuf::addTriangles(const std::vector<RrtTriangle> &input)
    {
        if (!input.empty())
        {
            m_triangles.assign(input.begin(), input.end());
            LOG_DEBUG("triangle size[%lu]", m_triangles.size());

            initBuf();

            glBindBuffer(GL_TEXTURE_BUFFER, m_bufId);
            glBufferData(GL_TEXTURE_BUFFER, m_triangles.size() * sizeof(RrtTriangle),
                         &m_triangles[0], GL_STATIC_DRAW);
            glBindTexture(GL_TEXTURE_BUFFER, m_texId);
            glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_bufId);
        }
    }

    void RrtTriBuf::bind(const std::shared_ptr<GfxShader> &shader, int texUnit)
    {
        if (m_triangles.empty())
        {
            LOG_ERR("array is empty");
            return;
        }
        RrtBuffer::bind(shader, texUnit);
    }
}

