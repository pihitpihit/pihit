#pragma once

#include <pthread.h>

#include "../autolibs/auto_result/auto_result.h"
#include "Primitive.h"

class Lock2: public PrimeNoncopy
{
	public:
		Lock2();
		~Lock2();

	public:
		void Enter();
		void Leave();

	protected:
		pthread_mutex_t lock_;
};

class Condition2: public Lock2
{
	public:
		static const int INFINITE = -1;
	public:
		Condition2();
		~Condition2();

	public:
		result_t Wait( int nTimeoutMs = Condition::INFINITE );
		void Wakeup( int bBroadcast = false );

	private:
		clockid_t clockid_;
		pthread_cond_t cond_;
		pthread_condattr_t attr_;
};

class CriticalSection2: public PrimeNoncopy
{
	public:
		CriticalSection2() = delete;
		CriticalSection2( Lock& lock );
		~CriticalSection2();

	private:
		Lock& lock_;
};

