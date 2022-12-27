#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "include/ClearLayer.h"
#include "ViewConv.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ClearLayer"

namespace view
{
    ClearLayer::ClearLayer(uint8_t order) : Layer(LayerType::BASIC, order),
                                            m_clearColor(0xffffffff)
    {
    }

    ClearLayer::~ClearLayer() = default;

    void ClearLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        const Viewport& port = conv->viewport();
        glViewport(port.x, port.y, port.width, port.height);
        glClearColor(RED_CHANNEL(m_clearColor), GREEN_CHANNEL(m_clearColor),
                     BLUE_CHANNEL(m_clearColor), ALPHA_CHANNEL(m_clearColor));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ClearLayer::createItems() {}
}

