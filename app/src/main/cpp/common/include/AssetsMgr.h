#ifndef RENDER_ASSETSMGR_H
#define RENDER_ASSETSMGR_H

#include <string>
#include <chrono>

#define ROOT "/storage/emulated/0/Android/data/com.lau.render/files/Documents"

enum PIC_TYPE : uint8_t
{
    PNG,
    JPG
};

enum SRC_TYPE : uint8_t
{
    MODEL,
    TEXTURE
};

namespace assets
{
    std::string getPath(const std::string& path, SRC_TYPE type)
    {
        switch (type) {
            case TEXTURE:
            {
                return ROOT + std::string("/") + "textures" + "/" + path;
            }
            case MODEL:
            default:
            {
                return ROOT + std::string("/") + "model" + "/" + path;
            }
        }
    }

    std::string getObj(const std::string &name)
    {
        std::string path(name);
        path.append("/");
        path.append(name);
        path.append(".obj");
        return getPath(path, SRC_TYPE::MODEL);
    }

    std::string getTexture(const std::string &name, PIC_TYPE type = PIC_TYPE::PNG)
    {
        std::string path(name);
        if (type == PIC_TYPE::JPG)
            path.append(".jpg");
        else
            path.append(".png");
        return getPath(path, SRC_TYPE::TEXTURE);
    }
}

#endif //RENDER_ASSETSMGR_H
