#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "GreRtxRender.h"
#include "RrtScene.h"
#include "Viewport.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreRtxRender"

namespace gre
{
    using namespace view;

    GreRtxRender::GreRtxRender() : GreRender()
    {
        m_scene = std::make_shared<RrtScene>();
    }

    GreRtxRender::~GreRtxRender() = default;

    void GreRtxRender::preUpd()
    {
        auto& viewport = m_conv->viewport();
        glViewport(viewport->x(), viewport->y(), viewport->width(), viewport->height());
    }

    void GreRtxRender::update()
    {
        m_scene->update(m_conv);
    }

    void GreRtxRender::postUpd() {}
}

