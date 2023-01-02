#include "GreRayTraceRender.h"
#include "RayTraceScene.h"

namespace gre
{
    using namespace view;

    GreRayTraceRender::GreRayTraceRender() : GreRender()
    {
        m_scene = std::make_shared<RayTraceScene>();
    }

    GreRayTraceRender::~GreRayTraceRender() = default;

    void GreRayTraceRender::preUpd() {}

    void GreRayTraceRender::update() {}

    void GreRayTraceRender::postUpd() {}
}

