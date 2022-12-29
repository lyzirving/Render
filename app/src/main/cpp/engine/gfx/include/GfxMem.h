#ifndef RENDER_GFXMEM_H
#define RENDER_GFXMEM_H

#include <string>
#include <cstdint>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "GfxPicMgr.h"

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
        friend class GfxPicMgr;
        PicMem() : m_width(0), m_height(0), m_channel(0), m_id(0), m_path() {}
        PicMem(const char *path) : m_width(0), m_height(0), m_channel(0), m_id(0), m_path(path) {}

        virtual ~PicMem()
        {
            destroyMem();
        }

        inline int32_t getWidth() { return m_width; }
        inline int32_t getHeight() { return m_height; }
        inline int32_t getChannel() { return m_channel; }
        inline uint32_t getId() { return m_id; }
        inline const std::string &getPath() { return m_path; }

        void createMem()
        {
            GfxPicMgr::get()->loadTex(m_path, m_id, m_width, m_height, m_channel);
        }

        void destroyMem()
        {
            GfxPicMgr::get()->release(m_path, m_id);
        }

    protected:
        int32_t m_width, m_height, m_channel;
        uint32_t m_id;
        std::string m_path;

    };
}

#endif //RENDER_GFXMEM_H
