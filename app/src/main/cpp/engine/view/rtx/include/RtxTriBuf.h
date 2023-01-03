#ifndef RENDER_RTXTRIBUF_H
#define RENDER_RTXTRIBUF_H

#include <vector>

#include "RtxBuffer.h"
#include "RtxStruct.h"

namespace view
{
    class RtxTriBuf : public RtxBuffer
    {
    public:
        RtxTriBuf();
        virtual ~RtxTriBuf();

        void addTriangles(const std::vector<RtxTriangle> &input);
        virtual void bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit) override;

    protected:
        std::vector<RtxTriangle> m_triangles;
    };
}

#endif //RENDER_RTXTRIBUF_H
