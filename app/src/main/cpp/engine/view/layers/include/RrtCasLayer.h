#ifndef RENDER_RRTCASLAYER_H
#define RENDER_RRTCASLAYER_H

#include "Layer.h"
#include "GfxDef.h"

namespace gfx
{
    class GfxShader;
    class RrtTriBuf;
}

namespace view
{
    class RrtCasLayer : public Layer
    {
    public:
        RrtCasLayer();

        virtual ~RrtCasLayer();

        virtual void update(const std::shared_ptr<ViewConv> &conv) override;

    protected:
        virtual void createItems() override;

    private:
        void drawCall();
        void initVideoMem();

        gfx::CasVert m_canvas[4];
        uint32_t m_casInd[6]{0, 1, 2, 2, 1, 3};
        uint32_t m_vao, m_vbo, m_ebo;
        uint32_t m_bgColor;

        std::shared_ptr<gfx::GfxShader> m_shader;
        std::shared_ptr<gfx::RrtTriBuf> m_triBuf;
    };
}

#endif //RENDER_RRTCASLAYER_H
