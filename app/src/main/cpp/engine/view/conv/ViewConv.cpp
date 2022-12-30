#include "ViewConv.h"
#include "Camera.h"
#include "Frustum.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ViewConv"

namespace view
{
    ViewConv::ViewConv() : m_model(1.f), m_view(1.f), m_project(1.f),
                           m_camera(new Camera), m_frustum(new Frustum),
                           m_port()
    {
    }

    ViewConv::~ViewConv()
    {
        m_camera.reset();
        m_frustum.reset();
    }

    void ViewConv::setViewport(int32_t x, int32_t y, int32_t width, int32_t height)
    {
        m_port.x = x;
        m_port.y = y;
        m_port.width = width;
        m_port.height = height;
    }
}
