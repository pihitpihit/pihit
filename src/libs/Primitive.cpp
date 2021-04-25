#include "Primitive.h"

#define NSEC_PER_MSEC   ( 1000 * 1000 )
#define NSEC_PER_SEC    ( 1000 * 1000 * 1000 )

TimeSpec TimeSpec::GetTime( int nRelativeMs )
{
	TimeSpec cTime;
	clock_gettime( CLOCK_REALTIME, &cTime );

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

