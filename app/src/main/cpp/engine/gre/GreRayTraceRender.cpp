#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "GreRayTraceRender.h"
#include "RayTraceScene.h"
#include "Viewport.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreRayTraceRender"

namespace gre
{
    using namespace view;

    GreRayTraceRender::GreRayTraceRender() : GreRender()
    {
        m_scene = std::make_shared<RayTraceScene>();
    }

    GreRayTraceRender::~GreRayTraceRender() = default;

    void GreRayTraceRender::preUpd()
    {
        auto& viewport = m_conv->viewport();
        glViewport(viewport->x(), viewport->y(), viewport->width(), viewport->height());
    }

    void GreRayTraceRender::update()
    {
        m_scene->update(m_conv);
    }

    void GreRayTraceRender::postUpd() {}
}

