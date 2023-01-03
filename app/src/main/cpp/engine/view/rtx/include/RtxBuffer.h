#ifndef RENDER_RTXBUFFER_H
#define RENDER_RTXBUFFER_H

#include <string>
#include <memory>

namespace gfx
{
    class GfxShader;
}

namespace view
{
    class RtxBuffer
    {
    public:
        RtxBuffer();
        virtual ~RtxBuffer();

        virtual void bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit);

    protected:
        void initBuf();

        std::string m_slot;
        uint32_t m_texId, m_bufId;
    };
}

#endif //RENDER_RTXBUFFER_H
