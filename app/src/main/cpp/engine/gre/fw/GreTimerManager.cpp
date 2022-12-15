#include "GreTimerManager.h"
#include "GreTimer.h"
#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreTimerManager"

namespace gre
{
    GreTimerManager::GreTimerManager() : m_timerList()
    {
    }

    GreTimerManager::~GreTimerManager()
    {
        if(!m_timerList.empty())
        {
            auto itr = m_timerList.begin();
            while (itr != m_timerList.end())
            {
                itr->reset();
                itr = m_timerList.erase(itr);
            }
        }
    }
}
