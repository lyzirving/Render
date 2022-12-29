#ifndef RENDER_CLEARLAYER_H
#define RENDER_CLEARLAYER_H

#include "Layer.h"

namespace view
{
    class ClearLayer : public Layer
    {
    public:
        ClearLayer(LayerOrder order = LayerOrder::LOW);
        virtual ~ClearLayer();

        virtual void update(const std::shared_ptr<ViewConv> &conv) override;

    protected:
        virtual void createItems() override;

    private:
        uint32_t m_clearColor;
    };
}

#endif //RENDER_CLEARLAYER_H
