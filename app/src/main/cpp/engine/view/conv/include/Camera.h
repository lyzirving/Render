#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <atomic>
#include "glm/glm.hpp"

namespace view
{
    class Camera
    {
    public:
        Camera(float radius = 3.f, float theta = 90.f, float phi = 0.f,
               float pitch = 0.f, float yaw = -90.f);
        virtual ~Camera();

        inline float getCamNear() { return m_camNear; }
        inline float getCamFar() { return m_camFar; }
        inline float getFov() { return m_fov; }
        inline void setCamNear(float near) { m_camNear = near; }
        inline void setCamFar(float far) { m_camFar = far; }
        inline void setFov(float fov) { m_fov = fov; }

        const glm::mat4 &getViewMt();
        const glm::vec3 &getCamPos();
        void setPosition(const glm::vec3 &pos);
        void setPosition(float r, float theta, float phi);

    private:
        void calcViewMt();
        void checkBound();

        glm::vec3 m_worldUp;
        glm::vec3 m_camPos;
        /**
         * @brief +x axis in camera local coordinate
         */
        glm::vec3 m_camRight;
        /**
         * @brief +y axis in camera local coordinate
         */
        glm::vec3 m_camUp;
        /**
         * @brief -z axis in camera local coordinate
         */
        glm::vec3 m_viewDir;
        /**
         * @brief pitch and yaw are euler angles that are used to decide the view direction(-z axis).
         *        note that x, y, z axis mentioned are OpenGL's standard right handed coordinate.
         *        pitch is rotated around x-axis, and it is an intersection angle between view direction and x-z plane.
         *        pitch is positive when it rotates from x-z plane to +y axis.
         *        pitch will decide how high the camera is from the x-z plane.
         */
        float m_viewDirPitch;
        /**
         * @brief pitch and yaw are euler angles that are used to decide the view direction(-z axis).
         *        note that the x, y, z axis mentioned are OpenGL's standard right handed coordinate.
         *        yaw is rotated around y-axis, and it should be only expressed in x-z plane.
         *        yaw is positive when it rotates from +x-axis to +z-axis.
         *        yaw will decide the rotation between camera and +z-axis.
         */
        float m_viewDirYaw;
        /**
         * @brief field of view in y direction, and it's valued in degree
         */
        float m_fov;
        float m_camNear, m_camFar;

        /*************************** spherical coordinate ***************************/
        /**
        * @brief (r, theta, phi) is used to express camera's position in spherical coordinate based on right handed system.
        *        theta is the polar angle(angle between +y axis and vector OC, O is origin, C is camera's position).
        *        theta ranges from [0, PI].
        *        when theta extends from +y to -y, its value changes from 0 to PI.
        */
        float m_theta;
        /**
        * @brief (r, theta, phi) is used to express camera's position in spherical coordinate based on right handed system.
        *        phi is the azimuth angle(angle between +z and projected position of camera on plane x-z).
        *        phi ranges from [0, 2PI].
        *        when projected position lies on the +z axis, phi is 0, and it increases in ccw to 2PI.
        */
        float m_phi;
        /**
         * @brief (r, theta, phi) is used to express camera's position in spherical coordinate based on right handed system.
         *        radius is the length of vector OC, while O is origin, C is camera's position.
         */
        float m_radius;

        glm::mat4 m_viewMt;

        std::atomic_bool m_change;
    };
}

#endif //RENDER_CAMERA_H
