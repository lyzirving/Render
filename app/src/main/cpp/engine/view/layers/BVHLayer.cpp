#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "BVHLayer.h"

#include "GfxShader.h"
#include "GfxShaderMgr.h"
#include "BVHNode.h"
#include "BVHBoundingBox.h"

#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHLayer"

using namespace gfx;

namespace view
{
    BVHLayer::BVHLayer(LayerOrder order) : Layer(LayerType::OBJ_LAYER, order),
                                           m_shader(nullptr), m_builder(nullptr), m_node(nullptr),
                                           m_box(new BVHBoundingBox)
    {
        BVHLayer::createItems();
    }

    BVHLayer::~BVHLayer()
    {
        m_shader.reset();
        m_builder.reset();
        m_node.reset();
        m_box.reset();
    }

    void BVHLayer::createItems()
    {
        m_builder = std::make_shared<BVHBuilder>("StanfordBunny", true);
        m_node = m_builder->build();
        int64_t start = systemTimeMs();
        m_box->getBVHBound(m_node);
        LOG_DEBUG("finish parsing bvh bound, cost[%.4f]s", (systemTimeMs() - start) / 1000.f);
    }

    void BVHLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        m_builder->drawDebug(conv);
        m_box->draw(conv);
    }
}

