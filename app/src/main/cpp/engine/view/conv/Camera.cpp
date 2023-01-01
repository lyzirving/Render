#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "ViewLib.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace view
{
    Camera::Camera(float radius, float theta, float phi, float pitch, float yaw) :
    m_worldUp(0, 1.f, 0),
    m_camPos(0.f), m_camRight(0.f), m_camUp(0.f),
    m_viewDir(0.f), m_viewDirPitch(pitch), m_viewDirYaw(yaw),
    m_theta(theta), m_phi(phi), m_radius(radius),
    m_viewMt(1.f), m_change(true)
    {
    }

    Camera::~Camera() = default;

    void Camera::calcViewMt()
    {
        if(m_change.load())
        {
            // compute the view direction by pitch and yaw
            // view direction is the opposite of camera front
            glm::vec3 viewDir;
            viewDir.x = cos(glm::radians(m_viewDirPitch)) * cos(glm::radians(m_viewDirYaw));
            viewDir.y = sin(glm::radians(m_viewDirPitch));
            viewDir.z = cos(glm::radians(m_viewDirPitch)) * sin(glm::radians(m_viewDirYaw));
            m_viewDir = glm::normalize(viewDir);

            // compute the right and up vector
            m_camRight = glm::normalize(glm::cross(m_viewDir, m_worldUp));
            m_camUp = glm::normalize(glm::cross(m_camRight, m_viewDir));

            sphericalToCartesian(m_radius, m_theta, m_phi, m_camPos);

            m_viewMt = glm::lookAt(m_camPos, m_camPos + m_viewDir, m_camUp);
            m_change.store(false);
        }
    }

    void Camera::checkBound()
    {
        if (m_phi > 360.f)
        {
            m_phi = (float)(int(m_phi) % 360);
        }
        else if(m_phi < 0.f)
        {
            m_phi = 360.f + m_phi;
        }

        if (m_theta > 178.f)
        {
            m_theta = 178.f;
        }
        else if (m_theta < 2.f)
        {
            m_theta = 2.f;
        }

        if (m_phi <= 90.f)
        {
            m_viewDirYaw = -90.f - m_phi;
        }
        else if(m_phi > 90.f && m_phi <= 180.f)
        {
            m_viewDirYaw = 90.f + 180.f - m_phi;
        }
        else if(m_phi > 180.f && m_phi <= 270.f)
        {
            m_viewDirYaw = 270.f - m_phi;
        }
        else
        {
            m_viewDirYaw = -(m_phi - 270.f);
        }

        if(m_theta <= 90.f)
        {
            m_viewDirPitch = -(90.f - m_theta);
        }
        else
        {
            m_viewDirPitch = 90.f - (180.f - m_theta);
        }
    }

    const glm::mat4 & Camera::getViewMat()
    {
        calcViewMt();
        return m_viewMt;
    }

    const glm::vec3 & Camera::getCamPos()
    {
        calcViewMt();
        return m_camPos;
    }

    void Camera::setPosition(const glm::vec3 &pos)
    {
        if(pos != m_camPos)
        {
            glm::vec3 spherical;
            cartesianToSpherical(pos, spherical);

            m_radius = spherical.x;
            m_theta = spherical.y;
            m_phi = spherical.z;
            checkBound();
            m_change.store(true);
        }
    }

    void Camera::setPosition(float r, float theta, float phi)
    {
        if(r != m_radius || theta != m_theta || phi != m_phi)
        {
            m_radius = r;
            m_theta = theta;
            m_phi = phi;
            checkBound();
            m_change.store(true);
        }
    }

}

