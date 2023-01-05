#ifndef RENDER_RRTBUFFER_H
#define RENDER_RRTBUFFER_H

#include <string>
#include <memory>

namespace gfx
{
    class GfxShader;

    class RrtBuffer
    {
    public:
        RrtBuffer();
        virtual ~RrtBuffer();

        virtual void bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit);

    protected:
        void initBuf();

        std::string m_slot;
        uint32_t m_texId, m_bufId;
    };
}

#endif //RENDER_RRTBUFFER_H
