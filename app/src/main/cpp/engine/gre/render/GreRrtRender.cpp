#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "GreRrtRender.h"
#include "RrtScene.h"
#include "Viewport.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreRrtRender"

using namespace view;

namespace gre
{
    GreRrtRender::GreRrtRender() : GreRender()
    {
        m_scene = std::make_shared<RrtScene>();
    }

    GreRrtRender::~GreRrtRender() = default;

    void GreRrtRender::preUpd()
    {
        auto& viewport = m_conv->viewport();
        glViewport(viewport->x(), viewport->y(), viewport->width(), viewport->height());
    }

    void GreRrtRender::update()
    {
        m_scene->update(m_conv);
    }

    void GreRrtRender::postUpd() {}
}

