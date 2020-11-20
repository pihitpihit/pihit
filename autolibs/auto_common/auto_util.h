#pragma once

#include <auto_windows.h>

namespace Plastics
{
	namespace Util
	{
		inline int Log( int base, int value )
		{
			int result = 1;
			while( base <= value )
			{
				value /= base;
				result++;
			}
			return result;
		};
	};
};
