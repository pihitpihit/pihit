#pragma once

namespace Plastics
{
	class TimeSpec: public timespec
	{
		public:
			static TimeSpec GetTime( int nRelativeMs = 0 );
	};
};
