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
        m_conv->setViewport(x, y, width, height);
    }

    void GreSceneRender::update()
    {
        if (!m_conv)
        {
            LOG_ERR("ViewConv is empty");
            assert(0);
        }
        m_scene->update(m_conv);
    }
}

