#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "ClearLayer.h"
#include "ViewConv.h"
#include "Viewport.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ClearLayer"

namespace view
{
    ClearLayer::ClearLayer(LayerOrder order) : Layer(LayerType::BASIC, order),
                                               m_clearColor(0xffffffff)
    {
    }

    ClearLayer::~ClearLayer() = default;

    void ClearLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        glClearColor(R_COMP(m_clearColor), G_COMP(m_clearColor), B_COMP(m_clearColor), A_COMP(m_clearColor));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ClearLayer::createItems() {}
}

