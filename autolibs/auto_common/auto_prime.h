#pragma once

namespace Plastics
{
	class Prime
	{
	};

	class Noncopy: public Prime
	{
		public:
			Noncopy() = default;
			~Noncopy() = default;
			Noncopy( const Noncopy& ) = delete;
			Noncopy& operator=( const Noncopy& ) = delete;
	};

	class Private: public Prime
	{
	};
};
