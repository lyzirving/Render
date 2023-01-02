#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "GreSceneRender.h"

#include "SightScene.h"
#include "Viewport.h"
#include "Camera.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreSceneRender"

namespace gre
{
    using namespace view;

    GreSceneRender::GreSceneRender() : GreRender()
    {
        m_scene = std::make_shared<SightScene>();
    }

    GreSceneRender::~GreSceneRender() = default;

    void GreSceneRender::preUpd()
    {
        auto& viewport = m_conv->viewport();
        glViewport(viewport->x(), viewport->y(), viewport->width(), viewport->height());

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const std::shared_ptr<Camera> &cam = m_conv->camera();
        cam->setPosition(6, 75.f, 20.f);
    }

    void GreSceneRender::update()
    {
        m_scene->update(m_conv);
    }

    void GreSceneRender::postUpd()
    {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    }
}

