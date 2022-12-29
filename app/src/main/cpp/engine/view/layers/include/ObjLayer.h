#ifndef RENDER_OBJLAYER_H
#define RENDER_OBJLAYER_H

#include "Layer.h"

namespace view
{
    class ObjLayer : public Layer
    {
    public:
        ObjLayer(LayerOrder order = LayerOrder::MID);

        virtual ~ObjLayer();
        virtual void update(const std::shared_ptr<ViewConv> &conv) override;

    protected:
        virtual void createItems() override;
    };
}

#endif //RENDER_OBJLAYER_H
