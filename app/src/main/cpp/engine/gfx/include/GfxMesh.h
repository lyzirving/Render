#ifndef RENDER_GFXMESH_H
#define RENDER_GFXMESH_H

#include <memory>
#include <vector>

#include "GfxDef.h"

namespace gfx
{
    class GfxShader;

    class GfxMesh : public VideoMem
    {
    public:
        GfxMesh();
        GfxMesh(const char *name);
        GfxMesh(GfxMesh &&other) noexcept;
        GfxMesh& operator =(GfxMesh &&other) noexcept;

        virtual ~GfxMesh();
        virtual void bind(bool force);
        virtual void draw(const std::shared_ptr<GfxShader> &shader);

        inline void setDrawMode(DrawMode mode) { m_drawMode = mode; }
        inline DrawMode getDrawMode() { return m_drawMode; }
        inline std::vector<Vertex> &getVertex() { return m_vertex; }
        inline std::vector<uint32_t> &getIndices() { return m_indices; }
        inline std::vector<std::shared_ptr<Texture>> &getTextures() { return m_textures; }
        inline const std::shared_ptr<Material> &getMaterial() { return m_material; }

        void release();

    protected:
        uint32_t getGlDrawMode();

        std::vector<Vertex> m_vertex;
        std::vector<uint32_t> m_indices;
        std::vector<std::shared_ptr<Texture>> m_textures;

        std::shared_ptr<Material> m_material;

        DrawMode m_drawMode;
        bool m_initialized;

    };
}

#endif //RENDER_GFXMESH_H
