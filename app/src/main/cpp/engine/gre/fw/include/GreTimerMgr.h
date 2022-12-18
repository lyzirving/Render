#ifndef RENDER_GRETIMERMGR_H
#define RENDER_GRETIMERMGR_H

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

    class GreTimerMgr
    {
    public:
        GreTimerMgr();
        virtual ~GreTimerMgr();

        inline int64_t getTimeout() { return m_minExpiration; }

        void addTimer(const std::shared_ptr<GreTimer>& timer);
        void removeTimer(const std::shared_ptr<GreTimer>& timer);
        void process();

    private:
        void computeMinExpiration();
        void processTimer();

        std::vector<std::shared_ptr<GreTimer>> m_timerArray;
        std::vector<std::shared_ptr<GreTimer>> m_tmpArray;
        std::atomic<bool> m_ticking;
        int64_t m_minExpiration;
    };
}

#endif //RENDER_GRETIMERMGR_H
