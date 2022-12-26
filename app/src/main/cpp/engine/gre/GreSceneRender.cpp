#include "GreSceneRender.h"

#include "SightScene.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreSceneRender"

namespace gre
{
    GreSceneRender::GreSceneRender() : m_scene(new view::SightScene)
    {}

    GreSceneRender::~GreSceneRender()
    {
        m_scene.reset();
    }

    void GreSceneRender::setViewport(int32_t x, int32_t y, int32_t width, int32_t height)
    {
        m_scene->setViewport(x, y, width, height);
    }

    void GreSceneRender::update()
    {
        m_scene->update();
    }
}

