#ifndef RENDER_GRETIMER_H
#define RENDER_GRETIMER_H

#include <atomic>
#include <optional>
#include "GreDef.h"
#include "GreObject.h"

namespace gre
{
    class GreTimer : public GreObject, IEvent
    {
    public:
        GreTimer(GreEventId key, int64_t intervalMs,
                 GrePriority priority = GrePriority::NORMAL);
        GreTimer(const GreTimer &other) = delete;

        virtual ~GreTimer();
        virtual void fire(PoolEvtArgType &&arg) override;

        GreTimer& operator=(const GreTimer &other) = delete;
        bool operator==(const GreTimer &other) const;

        void startTimer();
        void stopTimer();
        bool tick(int64_t sysTimeMs);

        inline GreEventId getKey() { return m_key; }
        inline GrePriority getPriority() { return m_priority; }
        inline uint32_t  getId() { return m_timerId; }
        inline bool isStart() { return m_isRunning.load(); }

    private:
        inline int64_t getExpiration() { return m_lastNotifyTime + m_intervalMs; }

        GreEventId m_key;
        GrePriority m_priority;
        int64_t m_intervalMs;
        int64_t m_lastNotifyTime;
        std::atomic<bool> m_isRunning;
        uint32_t m_timerId;

    };
}

#endif //RENDER_GRETIMER_H
