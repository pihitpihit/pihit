#pragma once

#include <pthread.h>

#include "../autolibs/auto_result/auto_result.h"
#include "Primitive.h"

class Lock: public PrimeNoncopy
{
	public:
		Lock();
		~Lock();

	public:
		void Enter();
		void Leave();

	protected:
		pthread_mutex_t lock_;
};

class Condition: public Lock
{
	public:
		static const int INFINITE = -1;
	public:
		Condition();
		~Condition();

	public:
		result_t Wait( int nTimeoutMs = Condition::INFINITE );
		void Wakeup( int bBroadcast = false );

	private:
		clockid_t clockid_;
		pthread_cond_t cond_;
		pthread_condattr_t attr_;
};

class CriticalSection: public PrimeNoncopy
{
	public:
		CriticalSection() = delete;
		CriticalSection( Lock& lock );
		~CriticalSection();

	private:
		Lock& lock_;
};

