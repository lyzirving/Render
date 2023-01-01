#ifndef RENDER_SPHERE_H
#define RENDER_SPHERE_H

#include "Model.h"

namespace view
{
    class Sphere : public Model
    {
    public:
        Sphere(const char *name, uint32_t horSegCnt = 50, uint32_t verSegCnt = 50, float radius = 0.1f);
        virtual ~Sphere();

        virtual void draw(const std::shared_ptr<ViewConv> &conv) override;

    protected:
        void build();

        uint32_t m_horSegCnt, m_verSegCnt;
        float m_radius;
        uint32_t m_color;

    private:
        uint32_t vertInd(uint32_t x, uint32_t y);

        inline uint32_t allVertNum() { return (2 + (m_verSegCnt - 1) * m_horSegCnt); }
        inline uint32_t allIndNum() { return (2 * m_horSegCnt * 3 + (m_verSegCnt - 2) * m_horSegCnt * 6); }
    };
}

#endif //RENDER_SPHERE_H
