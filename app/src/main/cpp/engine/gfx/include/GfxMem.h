#ifndef RENDER_GFXMEM_H
#define RENDER_GFXMEM_H

#include <cstdint>

namespace gfx
{
    class VideoMem
    {
    public:
        VideoMem() : m_vao(0), m_vbo(0), m_ebo(0) {};

        inline uint8_t getVAO() { return m_vao; }
        inline uint8_t getVBO() { return m_vbo; }
        inline uint8_t getEBO() { return m_ebo; }

    protected:
        uint8_t m_vao, m_vbo, m_ebo;
    };

    class PicMem
    {
    public:
        PicMem() : m_width(0), m_height(0), m_channel(0), m_id(0) {};

        inline uint32_t getWidth() { return m_width; }
        inline uint32_t getHeight() { return m_height; }
        inline uint8_t getChannel() { return m_channel; }
        inline uint8_t getId() { return m_id; }

    protected:
        uint32_t m_width, m_height;
        uint8_t m_channel;
        uint8_t m_id;
    };
}

#endif //RENDER_GFXMEM_H
