#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include "RtxTriBuf.h"

#include "GfxShader.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RtxTriBuf"

using namespace gfx;

namespace view
{
    RtxTriBuf::RtxTriBuf() : RtxBuffer(), m_triangles()
    {
        m_slot = RTX_SLOT_TRI;
    }

    RtxTriBuf::~RtxTriBuf()
    {
        std::vector<RtxTriangle>().swap(m_triangles);
    }

    void RtxTriBuf::addTriangles(const std::vector<RtxTriangle> &input)
    {
        if (!input.empty())
        {
            m_triangles.assign(input.begin(), input.end());

            initBuf();

            glBindBuffer(GL_TEXTURE_BUFFER, m_bufId);
            glBufferData(GL_TEXTURE_BUFFER, m_triangles.size() * sizeof(RtxTriangle),
                         &m_triangles[0], GL_STATIC_DRAW);
            glBindTexture(GL_TEXTURE_BUFFER, m_texId);
            glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_bufId);
        }
    }

    void RtxTriBuf::bind(const std::shared_ptr<GfxShader> &shader, int texUnit)
    {
        if (m_triangles.empty())
        {
            LOG_ERR("array is empty");
            return;
        }
        RtxBuffer::bind(shader, texUnit);
    }
}

