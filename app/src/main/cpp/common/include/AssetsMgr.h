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

class AssetsMgr
{
public:
    static AssetsMgr *get();

    static std::string getObj(const std::string &name);
    static std::string getTexture(const std::string &name, PIC_TYPE type = PIC_TYPE::PNG);

private:
    AssetsMgr();
    ~AssetsMgr();

    static std::string getPath(const std::string& path, SRC_TYPE type);
};

#endif //RENDER_ASSETSMGR_H
