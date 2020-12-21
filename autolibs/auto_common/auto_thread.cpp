#include <auto_thread.h>

using namespace Plastics;

AutoThread::AutoThread()
{
	(void)thread_;/*TODO:not used*/
}

AutoThread::~AutoThread()
{
}

result_t AutoThread::AddTask()
{
	return Result::Success;
}

result_t AutoThread::Run()
{
	return Result::Success;
}

result_t AutoThread::Join()
{
	return Result::Success;
}

result_t AutoThread::Detach()
{
	return Result::Success;
}

result_t AutoThread::Kill()
{
	return Result::Success;
}

result_t AutoThread::Pause()
{
	return Result::Success;
}

result_t AutoThread::Resume()
{
	return Result::Success;
}

result_t AutoThread::Cancel()
{
	return Result::Success;
}

ThreadId AutoThread::GetTid()
{
#ifdef  PLS_OS_WIN
	return (ThreadId)GetCurrentThreadId();
#else //PLS_OS_WIN
	return (ThreadId)pthread_self();
#endif//PLS_OS_WIN
}

