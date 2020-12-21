#pragma once

#ifdef  PLS_OS_WIN
#	include <windows.h>
#else //PLS_OS_WIN
#	include <thread>
#endif//PLS_OS_WIN
#include <auto_result.h>
#include <auto_sync.h>

namespace Plastics
{
#ifdef  PLS_OS_WIN
	typedef HANDLE HThread;
	typedef DWORD  ThreadId;
#else //PLS_OS_WIN
	typedef pthread_t HThread;
	typedef pthread_t ThreadId;
#endif//PLS_OS_WIN

	class AutoThread
	{
		public:
			AutoThread();
			~AutoThread();

		public:
			result_t AddTask();
			result_t Run();
			result_t Join();
			result_t Detach();
			result_t Kill();
			result_t Pause();
			result_t Resume();
			result_t Cancel();

		public:
			static ThreadId GetTid();

		private:


		private:
			std::thread* thread_;
			Condition cond_;
	};
};

