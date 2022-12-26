#ifndef RENDER_VIEWDEF_H
#define RENDER_VIEWDEF_H

namespace view
{
    #define RED_CHANNEL(color)    (((color >> 24u) & 0xffu) * 1.f / 255.f)
    #define GREEN_CHANNEL(color)  (((color >> 16u) & 0xffu) * 1.f / 255.f)
    #define BLUE_CHANNEL(color)   (((color >> 8u) & 0xffu) * 1.f / 255.f)
    #define ALPHA_CHANNEL(color)  ((color & 0xffu) * 1.f / 255.f)

    enum LayerType : uint8_t
    {
        BASIC,
        OBJ_LAYER,
        CNT
    };
}

#endif //RENDER_VIEWDEF_H
