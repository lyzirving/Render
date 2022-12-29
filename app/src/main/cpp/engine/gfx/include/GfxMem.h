#ifndef RENDER_GFXMEM_H
#define RENDER_GFXMEM_H

#include <cstdint>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

namespace gfx
{
    class VideoMem
    {
    public:
        VideoMem() : m_vao(0), m_vbo(0), m_ebo(0) {};

        inline uint32_t getVAO() { return m_vao; }
        inline uint32_t getVBO() { return m_vbo; }
        inline uint32_t getEBO() { return m_ebo; }

        void createMem()
        {
            if(m_vao != 0)
                glGenVertexArrays(1, &m_vao);
            if(m_vbo != 0)
                glGenBuffers(1, &m_vbo);
            if(m_ebo != 0)
                glGenBuffers(1, &m_ebo);
        }

        void destroyMem()
        {
            if(m_vbo != 0)
                glDeleteBuffers(1, &m_vbo);
            if(m_ebo != 0)
                glDeleteBuffers(1, &m_ebo);
            if(m_vao != 0)
                glDeleteVertexArrays(1, &m_vao);
        }

    protected:
        uint32_t m_vao, m_vbo, m_ebo;
    };

    class PicMem
    {
    public:
        PicMem() : m_width(0), m_height(0), m_channel(0), m_id(0) {};
        virtual ~PicMem()
        {
            destroyMem();
        }

        inline uint32_t getWidth() { return m_width; }
        inline uint32_t getHeight() { return m_height; }
        inline uint8_t getChannel() { return m_channel; }
        inline uint8_t getId() { return m_id; }

        void destroyMem()
        {
            if (m_id != 0)
            {
                glDeleteTextures(1, &m_id);
                m_id = 0;
            }
        }

    protected:
        uint32_t m_width, m_height;
        uint8_t m_channel;
        uint32_t m_id;
    };
}

#endif //RENDER_GFXMEM_H
