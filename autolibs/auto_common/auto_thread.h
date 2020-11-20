#pragma once

#ifdef  PLS_OS_WIN
#	include <windows.h>
#else //PLS_OS_WIN
#	include <thread>
#endif//PLS_OS_WIN

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
			static ThreadId GetTid();
	};
};

