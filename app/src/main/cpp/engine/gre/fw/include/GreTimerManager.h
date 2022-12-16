#ifndef RENDER_GRETIMERMANAGER_H
#define RENDER_GRETIMERMANAGER_H

#include <vector>

#include "GreTimer.h"

namespace gre
{
    struct
    {
        bool operator() (const std::shared_ptr<GreTimer>& lhs, const std::shared_ptr<GreTimer>& rhs)
        {
            if (!lhs) return false;
            if (!rhs) return true;
            return lhs->getPriority() >= rhs->getPriority();
        }
    } GreTimerCmpObj;

    class GreTimerManager
    {
    public:
        GreTimerManager();
        virtual ~GreTimerManager();

        void addTimer(const std::shared_ptr<GreTimer>& timer);
        void process();

    private:
        std::vector<std::shared_ptr<GreTimer>> m_timerList;
    };
}

#endif //RENDER_GRETIMERMANAGER_H
