#ifndef RENDER_FRUSTUM_H
#define RENDER_FRUSTUM_H

namespace view
{
    class Frustum
    {
    public:
        Frustum();
        Frustum(float left, float right, float top, float bottom, float near, float far);
        virtual ~Frustum();

        inline float left() { return m_left; }
        inline float right() { return m_right; }
        inline float top() { return m_top; }
        inline float bottom() { return m_bottom; }
        inline float near() { return m_near; }
        inline float far() { return m_far; }

        inline void setLeft(float left) { m_left = left; }
        inline void setRight(float right) { m_right = right; }
        inline void setTop(float top) { m_top = top; }
        inline void setBottom(float bottom) { m_bottom = bottom; }
        inline void setNear(float near) { m_near = near; }
        inline void setFar(float far) { m_far = far; }

    private:
        float m_left, m_right;
        float m_top, m_bottom;
        float m_near, m_far;
    };
}

#endif //RENDER_FRUSTUM_H
