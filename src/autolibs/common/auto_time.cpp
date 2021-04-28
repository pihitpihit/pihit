#include <auto_time.h>
#include <auto_result.h>
using namespace Plastics;

#define NSEC_PER_MSEC   ( 1000 * 1000 )
#define NSEC_PER_SEC    ( 1000 * 1000 * 1000 )

#ifdef PLS_OS_WIN
#	include <sysinfoapi.h>
#endif//PLS_OS_WIN

TimeSpec TimeSpec::GetTime( int nRelativeMs )
{
	TimeSpec cTime;

	ClockGetTime( ClockId::RealTime, &cTime );

	if( nRelativeMs == 0 )
	{
		return cTime;
	}
	if( nRelativeMs < 0 )
	{
		cTime.tv_nsec += ( nRelativeMs % 1000 ) * NSEC_PER_MSEC;
		cTime.tv_sec += nRelativeMs / 1000;
	}
	else//if( 0 < nRelativeMs )
	{
		cTime.tv_nsec += ( 1000 - nRelativeMs % 1000 ) * NSEC_PER_MSEC;
		cTime.tv_sec -= nRelativeMs / 1000 + 1;
	}

	cTime.tv_sec += cTime.tv_nsec / NSEC_PER_SEC;
	cTime.tv_nsec %= NSEC_PER_SEC;

	return cTime;
}

int TimeSpec::ClockGetTime( ClockId id, TimeSpec* tp )
{
#ifdef PLS_OS_WIN
	if( tp == nullptr )
	{
		return -1;
	}
	switch( id )
	{
		case ClockId::RealTime:
			{
				DWORD dwTickCount = GetTickCount();
				tp->tv_sec  = dwTickCount / 1000;
				tp->tv_nsec = (long)dwTickCount % 1000 * 1000;
			}
			break;
		default:
			/* TODO: Implement */
			;
	}
	return 0;
#else //PLS_OS_WIN
	clockid_t id2;
	switch( id )
	{
		case ClockId::RealTime:				id2 = CLOCK_REALTIME; 			break;
		case ClockId::Monotonic:			id2 = CLOCK_MONOTONIC;			break;
		case ClockId::ProcessCpuTimeId:		id2 = CLOCK_PROCESS_CPUTIME_ID;	break;
		case ClockId::ThreadCpuTimeId:		id2 = CLOCK_THREAD_CPUTIME_ID;	break;
		default:							id2 = CLOCK_MONOTONIC;			break;
			
	}
	return clock_gettime( id2, tp );
#endif//PLS_OS_WIN
}

result_t Tm::LocalTime( const time_t* timer, Tm* buf )
{
#ifdef  PLS_OS_WIN
	return localtime_s( ( struct tm* )buf, timer );
#else //PLS_OS_WIN
	struct tm* ptr = localtime_r( timer, buf );
	if( ptr == nullptr )
	{
		return Result::Internal;
	}
	return Result::Success;
#endif//PLS_OS_WIN
}

