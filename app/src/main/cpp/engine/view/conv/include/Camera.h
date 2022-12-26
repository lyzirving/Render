#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include "glm/glm.hpp"

namespace view
{
    class Camera
    {
    public:
        Camera();
        virtual ~Camera();

    private:
        glm::vec3 m_camPos, m_worldUp;
        /**
         * @brief -z axis in camera local coordinate
         */
        glm::vec3 m_viewDir;
        /**
         * @brief +x axis in camera local coordinate
         */
        glm::vec3 m_camRight;
        /**
         * @brief +y axis in camera local coordinate
         */
        glm::vec3 m_camUp;
        /**
         * @brief pitch and yaw are euler angles that are used to decide the view direction(-z axis).
         *        note that x, y, z axis mentioned are OpenGL's standard right handed coordinate.
         *        pitch is rotated around x-axis, and it is an intersection angle between view direction and x-z plane.
         *        pitch is positive when it rotates from x-z plane to +y axis.
         *        pitch will decide how high the camera is from the x-z plane.
         */
        float m_pitch;
        /**
         * @brief pitch and yaw are euler angles that are used to decide the view direction(-z axis).
         *        note that the x, y, z axis mentioned are OpenGL's standard right handed coordinate.
         *        yaw is rotated around y-axis, and it should be only expressed in x-z plane.
         *        yaw is positive when it rotates from +x-axis to +z-axis.
         *        yaw will decide the rotation between camera and +z-axis.
         */
        float m_yaw;
        /**
         * @brief field of view in y direction, and it's valued in degree
         */
        float m_fov;

        /*************************** spherical coordinate ***************************/
        /**
        * @brief Spherical coordinate(theta, thi, r) is used to express camera's position.
        *        Theta is the intersection angle between +z axis and camera projected pos on x-z plane.
        *        Theta ranges in [0, 2PI], when the projected pos lies align the +z axis, theta is 0.
        *        Theta increases in the counter clockwise direction.
        */
        float m_theta;
        /**
        * @brief Spherical coordinate(theta, thi, r) is used to express camera's position.
        *        if camera pos is defined by P, phi is the intersection angle between +y axis and OP.
        *        Phi ranges in [0, pi], when OP lies align the +y axis, phi is 0.
        *        When phi rotate from +y axis to -y axis, its value increases from 0 to pi.
        */
        float m_phi;
        /**
         * @brief Spherical coordinate(theta, thi, r) is used to express camera's position.
         *        m_radius refers to the radius of the sphere formed by the camera's position.
         */
        float m_radius;
    };
}

#endif //RENDER_CAMERA_H
