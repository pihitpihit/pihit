#include <errno.h>
#include <auto_sync.h>
#include <auto_time.h>
using namespace Plastics;

#ifdef  PLS_OS_WIN
#else //PLS_OS_WIN
#endif//PLS_OS_WIN

Lock::Lock()
{
#ifdef  PLS_OS_WIN
	InitializeCriticalSection( &lock_ );
#else //PLS_OS_WIN
	pthread_mutex_init( &lock_, NULL );
#endif//PLS_OS_WIN
}

Lock::~Lock()
{
#ifdef  PLS_OS_WIN
	DeleteCriticalSection( &lock_ );
#else //PLS_OS_WIN
	pthread_mutex_destroy( &lock_ );
#endif//PLS_OS_WIN
}

void Lock::Enter()
{
#ifdef  PLS_OS_WIN
	EnterCriticalSection( &lock_ );
#else //PLS_OS_WIN
	if( pthread_mutex_lock( &lock_ ) != 0 )
	{
		assert( !"Lock::Enter() failed." );
	}
#endif//PLS_OS_WIN
}

void Lock::Leave()
{
#ifdef  PLS_OS_WIN
	LeaveCriticalSection( &lock_ );
#else //PLS_OS_WIN
	if( pthread_mutex_unlock( &lock_ ) == EINVAL )
	{
		assert( !"Lock::Leave() failed." );
	}
#endif//PLS_OS_WIN
}

Condition::Condition()
{
#ifdef  PLS_OS_WIN
	cond_ = CreateEvent( nullptr, false, false, nullptr );
#else //PLS_OS_WIN
	pthread_condattr_init( &attr_ );
	pthread_cond_init( &cond_, &attr_ );
#endif//PLS_OS_WIN
}

Condition::~Condition()
{
#ifdef  PLS_OS_WIN
	CloseHandle( cond_ );
#else //PLS_OS_WIN
	pthread_cond_destroy( &cond_ );
	pthread_condattr_destroy( &attr_ );
#endif//PLS_OS_WIN
}

result_t Condition::Wait( int nTimeoutMs )
{
#ifdef  PLS_OS_WIN
	DWORD result = WaitForSingleObject( cond_, (DWORD)nTimeoutMs );
	return result_t( result );
#else //PLS_OS_WIN
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
#endif//PLS_OS_WIN
}

void Condition::Wakeup( int bBroadcast )
{
#ifdef  PLS_OS_WIN
	(void)bBroadcast;
	SetEvent( cond_ );
#else //PLS_OS_WIN
	if( bBroadcast )
	{
		pthread_cond_broadcast( &cond_ );
	}
	else
	{
		pthread_cond_signal( &cond_ );
	}
#endif//PLS_OS_WIN
}

CriticalSection::CriticalSection( Lock& lock ):
	lock_( lock )
{
	lock_.Enter();
}

CriticalSection::~CriticalSection()
{
	lock_.Leave();
}

