#ifndef RENDER_VIEWDEF_H
#define RENDER_VIEWDEF_H

namespace view
{
    #define R_COMP(color)    (((color >> 24u) & 0xffu) * 1.f / 255.f)
    #define G_COMP(color)  (((color >> 16u) & 0xffu) * 1.f / 255.f)
    #define B_COMP(color)   (((color >> 8u) & 0xffu) * 1.f / 255.f)
    #define A_COMP(color)  ((color & 0xffu) * 1.f / 255.f)

    enum LayerType : uint8_t
    {
        BASIC,
        OBJ_LAYER,
        CANVAS,
        CNT
    };

    enum LayerOrder : uint8_t
    {
        LOW,
        MID,
        MID_1,
        TOP
    };

    enum AdjFlag : uint8_t
    {
        ADJ_CENTER = 0b00000001,
        ADJ_SCALE  = 0b00000010
    };
}

#endif //RENDER_VIEWDEF_H
