#pragma once

#include <thread>

namespace Plastics
{
	typedef pthread_t HThread;
	typedef pthread_t ThreadId;

	class AutoThread
	{
		public:
			static ThreadId GetTid();
	};
};

