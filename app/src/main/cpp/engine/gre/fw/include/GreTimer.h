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
        enum Status : uint8_t
        {
            IDLE,
            STARTED,
            PAUSED,
            STOPPED,
            CNT
        };

        GreTimer(GreEventId key, int64_t intervalMs,
                 GrePriority priority = GrePriority::NORMAL);
        GreTimer(const GreTimer &other) = delete;

        virtual ~GreTimer();
        virtual void fire(const PoolEvtArg &arg) override;

        GreTimer& operator=(const GreTimer &other) = delete;
        bool operator==(const GreTimer &other) const;

        bool tick(int64_t sysTimeMs);

        inline GreEventId getKey() { return m_key; }
        inline GrePriority getPriority() { return m_priority; }
        inline uint32_t  getId() { return m_timerId; }
        inline int64_t getExpiration() { return m_lastNotifyTime + m_intervalMs; }
        inline bool isRunning() { return m_status.load() == Status::STARTED; }
        inline bool isPaused() { return m_status.load() == Status::PAUSED; }
        inline bool isStopped() { return m_status.load() == Status::STOPPED; }
        inline void start() { m_status = Status::STARTED; }
        inline void pause() { m_status = Status::PAUSED; }
        inline void stop() { m_status = Status::STOPPED; }

    private:
        GreEventId m_key;
        GrePriority m_priority;
        int64_t m_intervalMs;
        int64_t m_lastNotifyTime;
        uint32_t m_timerId;
        std::atomic<uint8_t> m_status;

    };
}

#endif //RENDER_GRETIMER_H
