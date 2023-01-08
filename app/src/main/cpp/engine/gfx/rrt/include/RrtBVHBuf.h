#ifndef RENDER_RRTBVHBUF_H
#define RENDER_RRTBVHBUF_H

#include <vector>

#include "RrtBuffer.h"
#include "RrtStruct.h"

namespace gfx
{
    class RrtBVHBuf : public RrtBuffer
    {
    public:
        RrtBVHBuf();
        virtual ~RrtBVHBuf();

        void addNodes(const std::vector<RrtBVHNode> &input);
        virtual void bind(const std::shared_ptr<gfx::GfxShader> &shader, int texUnit) override;

        inline int32_t nodesCnt() { return m_nodes.size(); }

    protected:
        std::vector<RrtBVHNode> m_nodes;
    };
}

#endif //RENDER_RRTBVHBUF_H
