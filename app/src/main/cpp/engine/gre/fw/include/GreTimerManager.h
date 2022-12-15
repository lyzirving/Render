#ifndef RENDER_GRETIMERMANAGER_H
#define RENDER_GRETIMERMANAGER_H

#include <cstdint>
#include <vector>
#include <memory>

namespace gre
{
    class GreTimer;

    class GreTimerManager
    {
    public:
        GreTimerManager();
        virtual ~GreTimerManager();

    private:
        std::vector<std::shared_ptr<GreTimer>> m_timerList;
    };
}

#endif //RENDER_GRETIMERMANAGER_H
