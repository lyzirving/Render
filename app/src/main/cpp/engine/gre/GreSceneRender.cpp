#include "GreSceneRender.h"

#include "Scene.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreSceneRender"

namespace gre
{
    GreSceneRender::GreSceneRender() : m_scene(new view::Scene)
    {}

    GreSceneRender::~GreSceneRender()
    {
        m_scene.reset();
    }

    void GreSceneRender::update()
    {
        m_scene->update();
    }
}

