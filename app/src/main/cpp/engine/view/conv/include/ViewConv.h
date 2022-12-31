#ifndef RENDER_VIEWCONV_H
#define RENDER_VIEWCONV_H

#include <memory>

#include "glm/glm.hpp"

namespace view
{
    class Camera;
    class Frustum;

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

        const glm::mat4 &getViewMat();
        const glm::mat4 &getProjectMat();

        inline const Viewport &viewport() { return m_port; }
        /**
         * const std::shared_ptr<Camera> references to *ptr const
         * @return return a const pointer. content pointed by this ptr can be modified,
         *         but the pointer itself can not.
         */
        inline const std::shared_ptr<Camera> &camera() { return m_camera; }
        inline const std::shared_ptr<Frustum> &frustum() { return m_frustum; }

        void setViewport(int32_t x, int32_t y, int32_t width, int32_t height);

    private:
        glm::mat4 m_viewMat, m_projectMat;

        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<Frustum> m_frustum;
        Viewport m_port;
    };
}

#endif //RENDER_VIEWCONV_H
