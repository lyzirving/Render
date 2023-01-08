#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include "RrtBVHBuf.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtBVHBuf"

namespace gfx
{
    RrtBVHBuf::RrtBVHBuf() : RrtBuffer(), m_nodes()
    {
        m_slot = RRT_SLOT_BVH;
    }

    RrtBVHBuf::~RrtBVHBuf()
    {
        std::vector<RrtBVHNode>().swap(m_nodes);
    }

    void RrtBVHBuf::addNodes(const std::vector<RrtBVHNode> &input)
    {
        if (!input.empty())
        {
            m_nodes.assign(input.begin(), input.end());
            LOG_DEBUG("BVH node size[%lu]", m_nodes.size());

            initBuf();

            glBindBuffer(GL_TEXTURE_BUFFER, m_bufId);
            glBufferData(GL_TEXTURE_BUFFER, m_nodes.size() * sizeof(RrtBVHNode), &m_nodes[0], GL_STATIC_DRAW);
            glBindTexture(GL_TEXTURE_BUFFER, m_texId);
            glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_bufId);
        }
    }

    void RrtBVHBuf::bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit)
    {
        if (m_nodes.empty())
        {
            LOG_ERR("nodes is empty");
            return;
        }
        RrtBuffer::bind(shader, texUnit);
    }
}

