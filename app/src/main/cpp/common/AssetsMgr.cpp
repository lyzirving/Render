#include <mutex>

#include "AssetsMgr.h"

std::mutex g_mutex{};
AssetsMgr *g_mgr{nullptr};

AssetsMgr *AssetsMgr::get()
{
    if (!g_mgr) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_mgr)
            g_mgr = new AssetsMgr;
    }
    return g_mgr;
}

AssetsMgr::AssetsMgr() = default;

AssetsMgr::~AssetsMgr() = default;

std::string AssetsMgr::getPath(const std::string &path, SRC_TYPE type)
{
    switch (type)
    {
        case SHADER:
        {
            return ROOT + std::string("/") + "shader" + "/" + path;
        }
        case TEXTURE:
        {
            return ROOT + std::string("/") + "texture" + "/" + path;
        }
        case MODEL:
        default:
        {
            return ROOT + std::string("/") + "model" + "/" + path;
        }
    }
}

std::string AssetsMgr::getObj(const std::string &name)
{
    std::string path(name);
    path.append("/");
    path.append(name);
    path.append(".obj");
    return getPath(path, SRC_TYPE::MODEL);
}

std::string AssetsMgr::getTexture(const std::string &name, PIC_TYPE type)
{
    std::string path(name);
    if (type == PIC_TYPE::JPG)
        path.append(".jpg");
    else
        path.append(".png");
    return getPath(path, SRC_TYPE::TEXTURE);
}

std::string AssetsMgr::getVert(const std::string &name)
{
    std::string path(name);
    path.append(".vert");
    return getPath(path, SRC_TYPE::SHADER);
}

std::string AssetsMgr::getFrag(const std::string &name)
{
    std::string path(name);
    path.append(".frag");
    return getPath(path, SRC_TYPE::SHADER);
}

