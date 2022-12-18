#ifndef RENDER_GREPROXY_H
#define RENDER_GREPROXY_H

#include <cstdint>

struct ANativeWindow;

namespace gre
{
    void PROXY_attachView(int32_t id);

    bool PROXY_attachSurface(int32_t id, ANativeWindow *window);

    void PROXY_detachView(int32_t id);
}

#endif //RENDER_GREPROXY_H
