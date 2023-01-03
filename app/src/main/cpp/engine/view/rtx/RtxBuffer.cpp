#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include "RtxBuffer.h"

#include "GfxShader.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RtxBuffer"

namespace view
{
    RtxBuffer::RtxBuffer() : m_slot(), m_texId(0), m_bufId(0) {}

    RtxBuffer::~RtxBuffer()
    {
        if (m_bufId != 0)
            glDeleteBuffers(1, &m_bufId);

        if(m_texId != 0)
            glDeleteTextures(1, &m_texId);

        m_bufId = 0;
        m_texId = 0;
    }

    void RtxBuffer::initBuf()
    {
        if(m_bufId == 0)
            glGenBuffers(1, &m_bufId);
        if(m_texId == 0)
            glGenTextures(1, &m_texId);
    }

    void RtxBuffer::bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit)
    {
        if(!shader)
        {
            return;
        }
        if (m_slot.empty())
        {
            LOG_ERR("attr slot is empty");
            return;
        }
        if(m_texId == 0 || m_bufId == 0)
        {
            LOG_ERR("buf[%s] invalid id, tex[%u], buf[%u]", m_slot.c_str(), m_texId, m_bufId);
            return;
        }
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindTexture(GL_TEXTURE_BUFFER, m_texId);
        shader->setInt(m_slot, texUnit);
    }
}

