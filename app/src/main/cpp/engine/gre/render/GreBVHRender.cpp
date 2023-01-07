#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "GreBVHRender.h"

#include "Viewport.h"
#include "BVHScene.h"
#include "Camera.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreBVHRender"

using namespace view;

namespace gre
{
    GreBVHRender::GreBVHRender() : GreRender()
    {
        m_scene = std::make_shared<BVHScene>();
    }

    GreBVHRender::~GreBVHRender() = default;

    void GreBVHRender::preUpd()
    {
        auto& viewport = m_conv->viewport();
        glViewport(viewport->x(), viewport->y(), viewport->width(), viewport->height());

        const std::shared_ptr<Camera> &cam = m_conv->camera();
        cam->setPosition(8, 75.f, 20.f);
    }

    void GreBVHRender::update()
    {
        m_scene->update(m_conv);
    }

    void GreBVHRender::postUpd() {}
}
