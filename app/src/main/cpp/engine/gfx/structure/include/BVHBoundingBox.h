#ifndef RENDER_BVHBOUNDINGBOX_H
#define RENDER_BVHBOUNDINGBOX_H

#include "GfxMesh.h"

namespace view
{
    class ViewConv;
}

namespace gfx
{
    class BVHNode;

    class BVHBoundingBox : public gfx::GfxMesh {
    public:
        BVHBoundingBox();
        BVHBoundingBox(const char *name);

        virtual ~BVHBoundingBox();
        virtual void bind(bool force) override;
        virtual void createMem() override;

        virtual void draw(const std::shared_ptr<view::ViewConv> &conv);
        void getBVHBound(const std::shared_ptr<gfx::BVHNode> &node);

    private:
        void adjModel();
        void itrBVH(const std::shared_ptr<gfx::BVHNode> &node);
        void parseBVH(const std::shared_ptr<gfx::BVHNode> &node);

        glm::vec3 m_max, m_min;
        glm::mat4 m_modelMt;
    };
}

#endif //RENDER_BVHBOUNDINGBOX_H
