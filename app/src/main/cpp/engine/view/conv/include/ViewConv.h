#ifndef RENDER_VIEWCONV_H
#define RENDER_VIEWCONV_H

#include "glm/glm.hpp"

namespace view
{
    struct Viewport
    {
        int32_t x, y;
        int32_t width, height;

        Viewport() : x(0), y(0), width(0), height(0) {}
        ~Viewport() = default;
    };

    class ViewConv
    {
    public:
        ViewConv();
        virtual ~ViewConv();

        inline const glm::mat4 &mtModel() { return m_model; }
        inline const glm::mat4 &mtView() { return m_view; }
        inline const glm::mat4 &mtProject() { return m_project; }
        inline const Viewport &viewport() { return m_port; }

        inline void setModelMt(const glm::mat4 &model) { m_model = model; }
        inline void setViewMt(const glm::mat4 &view) { m_view = view; }
        inline void setProjectMt(const glm::mat4 &project) { m_project = project; }

        void setViewport(int32_t x, int32_t y, int32_t width, int32_t height);

    private:
        glm::mat4 m_model, m_view, m_project;
        Viewport m_port;
    };
}

#endif //RENDER_VIEWCONV_H