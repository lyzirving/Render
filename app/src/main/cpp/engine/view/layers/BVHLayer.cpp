#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "BVHLayer.h"

#include "GfxShader.h"
#include "GfxShaderMgr.h"
#include "BVHNode.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHLayer"

using namespace gfx;

namespace view
{
    BVHLayer::BVHLayer() : m_shader(nullptr), m_builder(nullptr), m_node(nullptr)
    {
        BVHLayer::createItems();
    }

    BVHLayer::~BVHLayer()
    {
        m_shader.reset();
        m_builder.reset();
        m_node.reset();
    }

    void BVHLayer::createItems()
    {
        m_builder = std::make_shared<BVHBuilder>("StanfordBunny", true);
        m_node = m_builder->build();
    }

    void BVHLayer::update(const std::shared_ptr<ViewConv> &conv)
    {

    }
}

