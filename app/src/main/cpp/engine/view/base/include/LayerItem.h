#ifndef RENDER_LAYERITEM_H
#define RENDER_LAYERITEM_H

#include <memory>
#include <glm/glm.hpp>

namespace view
{
    class ViewConv;

    class LayerItem
    {
    public:
        LayerItem() : m_mtAdj(1.f), m_mtModel(1.f),
                      m_mtTrans(1.f), m_mtScale(1.f), m_mtRotate(1.f)
        {
        }
        virtual ~LayerItem() = default;

        virtual void draw(const std::shared_ptr<ViewConv> &conv) = 0;
        virtual void release() {}

    protected:
        glm::mat4 m_mtAdj;
        glm::mat4 m_mtModel;
        glm::mat4 m_mtTrans, m_mtScale, m_mtRotate;
    };
}

#endif //RENDER_LAYERITEM_H
