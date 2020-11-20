#include <auto_thread.h>

using namespace Plastics;

ThreadId AutoThread::GetTid()
{
#ifdef  PLS_OS_WIN
	return (ThreadId)GetCurrentThreadId();
#else //PLS_OS_WIN
	return (ThreadId)pthread_self();
#endif//PLS_OS_WIN
}

