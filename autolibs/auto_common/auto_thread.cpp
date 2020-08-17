#include <auto_thread.h>

using namespace Plastics;

ThreadId AutoThread::GetTid()
{
	return (ThreadId)pthread_self();
}

