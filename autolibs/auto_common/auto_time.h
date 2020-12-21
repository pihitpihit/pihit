#pragma once

#include <time.h>
#include <auto_result.h>

namespace Plastics
{
	class TimeSpec: public timespec
	{
		public:
			enum ClockId
			{
				RealTime				,
				Monotonic				,
				ProcessCpuTimeId		,
				ThreadCpuTimeId			,
				//RealTimeCoarse			,
			};

		public:
			static TimeSpec GetTime( int nRelativeMs = 0 );
			static int ClockGetTime( ClockId id, TimeSpec* tp );
	};

	class Tm: public tm
	{
		public:
			static result_t LocalTime( const time_t* timer, Tm* buf );
	};
};
