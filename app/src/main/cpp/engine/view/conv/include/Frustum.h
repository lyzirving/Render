#ifndef RENDER_FRUSTUM_H
#define RENDER_FRUSTUM_H

#include <atomic>
#include <glm/glm.hpp>

namespace view
{
    class Frustum
    {
    public:
        Frustum();
        Frustum(float width, float height, float near, float far);
        virtual ~Frustum();

        inline float near() { return m_near; }
        inline float far() { return m_far; }
        inline float fov() { return glm::degrees(m_fov); }
        inline float aspect() { return m_aspect; };
        inline bool isChanged() { return m_change.load(); }

        const glm::mat4 &getProjectMat();
        void setWidth(float width);
        void setHeight(float height);
        void setNear(float near);
        void setFar(float far);

    private:
        void calcProjectMat();

        float m_left, m_right;
        float m_top, m_bottom;
        float m_near, m_far;
        float m_fov, m_aspect;
        std::atomic_bool m_change;
        glm::mat4 m_projectMat;
    };
}

#endif //RENDER_FRUSTUM_H
