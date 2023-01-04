#ifndef RENDER_RRTTRIBUF_H
#define RENDER_RRTTRIBUF_H

#include <vector>

#include "RrtBuffer.h"
#include "RrtStruct.h"

namespace view
{
    class RrtTriBuf : public RrtBuffer
    {
    public:
        RrtTriBuf();
        virtual ~RrtTriBuf();

        void addTriangles(const std::vector<RtxTriangle> &input);
        virtual void bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit) override;

        inline int32_t triangleCnt() { return m_triangles.size(); }

    protected:
        std::vector<RtxTriangle> m_triangles;
    };
}

#endif //RENDER_RRTTRIBUF_H
