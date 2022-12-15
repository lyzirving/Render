#ifndef RENDER_GRETIMER_H
#define RENDER_GRETIMER_H

#include <atomic>
#include "GreDef.h"
#include "GreObject.h"

namespace gre
{
    class GreTimer : public GreObject, IEvent
    {
    public:
        GreTimer(GreTimerKey key, int64_t intervalMs,
                 GrePriority priority = GrePriority::NORMAL);

        virtual ~GreTimer();
        virtual void fire(std::shared_ptr<GreEventArg> &arg) override;

        void start();
        void stop();
        bool tick(int64_t sysTimeMs);

        inline GreTimerKey getKey() { return m_key; }
        inline GrePriority getPriority() { return m_priority; }

    private:
        inline int64_t getExpiration() { return m_lastNotifyTime + m_intervalMs; }

        GreTimerKey m_key;
        GrePriority m_priority;
        int64_t m_intervalMs;
        int64_t m_lastNotifyTime;
        std::atomic<bool> m_isRunning;
    };
}

#endif //RENDER_GRETIMER_H
