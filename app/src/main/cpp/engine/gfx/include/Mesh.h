#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <memory>
#include <vector>

#include "GfxDef.h"

namespace gfx
{
    class Shader;

    class Mesh : public VideoMem
    {
    public:
        Mesh();
        Mesh(Mesh &&other) noexcept;
        Mesh& operator =(Mesh &&other) noexcept;

        virtual ~Mesh();
        virtual void bind(bool force);
        virtual void draw(const std::shared_ptr<Shader> &shader);

        inline void setDrawMode(DrawMode mode) { m_drawMode = mode; }
        inline DrawMode getDrawMode() { return m_drawMode; }
        inline std::vector<Vertex> &getVertex() { return m_vertex; }
        inline std::vector<uint32_t> &getIndices() { return m_indices; }
        inline std::vector<std::shared_ptr<Texture>> &getTextures() { return m_textures; }

        void release();

    protected:
        std::vector<Vertex> m_vertex;
        std::vector<uint32_t> m_indices;
        std::vector<std::shared_ptr<Texture>> m_textures;

        std::shared_ptr<Material> m_material;

        DrawMode m_drawMode;
        bool m_initialized;
    };
}

#endif //RENDER_MESH_H