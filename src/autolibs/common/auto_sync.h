#pragma once

#ifdef  PLS_OS_WIN
#	include <stdlib.h>
#	include <windows.h>
#else //PLS_OS_WIN
#	include <pthread.h>
#endif//PLS_OS_WIN
#include <auto_result.h>
#include <auto_prime.h>
#include <auto_util.h>
#include <auto_windows.h>

namespace Plastics
{
	class Lock: public Noncopy
	{
	public:
		Lock();
		~Lock();

	public:
		void Enter();
		void Leave();

	protected:
#ifdef  PLS_OS_WIN
		CRITICAL_SECTION	lock_;
#else //PLS_OS_WIN
		pthread_mutex_t 	lock_;
#endif//PLS_OS_WIN
	};

	class Condition: public Lock
	{
		public:
			static const int TIME_INFINITE = -1;
		public:
			Condition();
			~Condition();

		public:
			result_t Wait( int nTimeoutMs = Condition::TIME_INFINITE );
			void Wakeup( int bBroadcast = false );

		private:
#ifdef  PLS_OS_WIN
			HANDLE				cond_;
#else //PLS_OS_WIN
			pthread_cond_t 		cond_;
			pthread_condattr_t 	attr_;
#endif//PLS_OS_WIN
	};

	class CriticalSection: public Noncopy
	{
		public:
			CriticalSection() = delete;
			CriticalSection( Lock& lock );
			~CriticalSection();

		private:
			Lock& lock_;
	};
};
