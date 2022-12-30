#include "Frustum.h"

namespace view
{
    Frustum::Frustum() : m_left(0.f), m_right(0.f), m_top(0.f), m_bottom(0.f),
                         m_near(0.f), m_far(0.f)
    {
    }

    Frustum::Frustum(float left, float right, float top, float bottom, float near, float far)
    : m_left(left), m_right(right), m_top(top), m_bottom(bottom), m_near(near), m_far(far)
    {
    }

    Frustum::~Frustum() = default;
}

