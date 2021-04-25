#pragma once
#include <time.h>

class PrimeNoncopy
{
	public:
		PrimeNoncopy() = default;
		~PrimeNoncopy() = default;
		PrimeNoncopy( const PrimeNoncopy& ) = delete;
		PrimeNoncopy& operator=( const PrimeNoncopy& ) = delete;
};

class TimeSpec: public timespec
{
	public:
		static TimeSpec GetTime( int nRelativeMs = 0 );
};
