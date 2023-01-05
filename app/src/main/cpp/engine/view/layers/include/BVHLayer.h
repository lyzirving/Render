#ifndef RENDER_BVHLAYER_H
#define RENDER_BVHLAYER_H

#include "Layer.h"

namespace gfx
{
    class GfxShader;
    class BVHBuilder;
    class BVHNode;
}

namespace view
{
    class BVHLayer : public Layer
    {
    public:
        BVHLayer();

        virtual ~BVHLayer();

        virtual void update(const std::shared_ptr<ViewConv> &conv) override;

    protected:
        virtual void createItems() override;

    private:
        std::shared_ptr<gfx::GfxShader> m_shader;
        std::shared_ptr<gfx::BVHBuilder> m_builder;
        std::shared_ptr<gfx::BVHNode> m_node;
    };
}

#endif //RENDER_BVHLAYER_H
