#include "NreThread.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "NreThread"

NreThread::NreThread(const char *name) : Thread(name, true)
{
}

NreThread::~NreThread() = default;