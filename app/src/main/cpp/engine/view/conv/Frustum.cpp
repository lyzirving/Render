#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "Frustum.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Frustum"

namespace view
{
    Frustum::Frustum() : m_left(0.f), m_right(0.f), m_top(0.f), m_bottom(0.f),
                         m_near(0.1f), m_far(10.f), m_fov(0.f), m_aspect(0.f),
                         m_change(true), m_projectMat(1.f)
    {
    }

    Frustum::Frustum(float width, float height, float near, float far)
    : m_left(-width / 2.f), m_right(width / 2.f), m_top(height / 2.f), m_bottom(-height / 2.f),
      m_near(near), m_far(far), m_fov(0.f), m_aspect(0.f),
      m_change(true), m_projectMat(1.f)
    {
        calcProjectMat();
    }

    Frustum::~Frustum() = default;

    void Frustum::calcProjectMat()
    {
        if (m_change.load())
        {
            if (m_right == 0.f || m_top == 0.f || m_near <= 0.f || m_far <= 0.f)
            {
                LOG_ERR("err, invalid value for frustum, left[%f], right[%f], top[%f], bottom[%f], "
                        "near[%f] and far[%f]", m_left, m_right, m_top, m_bottom, m_near, m_far);
                assert(0);
            }

            m_fov = std::abs(std::atan(m_top / m_near));
            m_aspect = std::abs(m_right / m_top);
            m_projectMat = glm::perspective(m_fov, m_aspect, m_near, m_far);
            m_change.store(false);
        }
    }

    const glm::mat4 & Frustum::getProjectMat()
    {
        calcProjectMat();
        return m_projectMat;
    }

    void Frustum::setWidth(float width)
    {
        if(width / 2.f != m_right)
        {
            m_right = width / 2.f;
            m_left = -m_right;
            m_change.store(true);
            LOG_DEBUG("left[%.2f] and right[%.2f] changes", m_left, m_right);
        }
    }

    void Frustum::setHeight(float height)
    {
        if(height / 2.f != m_top)
        {
            m_top = height / 2.f;
            m_bottom = -m_top;
            m_change.store(true);
            LOG_DEBUG("bottom[%.2f] and top[%.2f] changes", m_bottom, m_top);
        }
    }

    void Frustum::setNear(float near)
    {
        if (near != m_near)
        {
            m_near = near;
            m_change.store(true);
            LOG_DEBUG("near[%.2f] changes", m_near);
        }
    }

    void Frustum::setFar(float far)
    {
        if (far != m_far)
        {
            m_far = far;
            m_change.store(true);
            LOG_DEBUG("far[%f] changes", m_far);
        }
    }
}

