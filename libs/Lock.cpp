#include <errno.h>
#include "Lock.h"

Lock2::Lock()
{
	pthread_mutex_init( &lock_, NULL );
}

Lock2::~Lock()
{
	pthread_mutex_destroy( &lock_ );
}

void Lock2::Enter()
{
	if( pthread_mutex_lock( &lock_ ) != 0 )
	{
		assert( !"Lock::Enter() failed." );
	}
}

void Lock2::Leave()
{
	if( pthread_mutex_unlock( &lock_ ) == EINVAL )
	{
		assert( !"Lock::Leave() failed." );
	}
}

Condition2::Condition():
	clockid_( CLOCK_REALTIME )
{
	pthread_condattr_init( &attr_ );
	pthread_cond_init( &cond_, &attr_ );
}

Condition2::~Condition()
{
	pthread_cond_destroy( &cond_ );
	pthread_condattr_destroy( &attr_ );
}

result_t Condition2::Wait( int nTimeoutMs )
{
	int error;

	if( 0 <= nTimeoutMs )
	{
		TimeSpec cTime = TimeSpec::GetTime( nTimeoutMs );
		error = pthread_cond_timedwait( &cond_, &lock_, &cTime);
	}
	else
	{
		error = pthread_cond_wait( &cond_, &lock_ );
	}

	return result_t( error );
}

void Condition2::Wakeup( int bBroadcast )
{
	if( bBroadcast )
	{
		pthread_cond_broadcast( &cond_ );
	}
	else
	{
		pthread_cond_signal( &cond_ );
	}
}

CriticalSection2::CriticalSection( Lock& lock ):
	lock_( lock )
{
	lock_.Enter();
}

CriticalSection2::~CriticalSection()
{
	lock_.Leave();
}

