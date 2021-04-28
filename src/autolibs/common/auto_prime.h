#pragma once
#include <functional>

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

	class AutoFinalizer: public Prime
	{
	public:
		AutoFinalizer( std::function<void()> finalizer );
		~AutoFinalizer();
	private:
		std::function<void()> finalizer_;
	};
};

#define AutoFinalize( _fin )\
	Plastics::AutoFinalizer _af_( _fin )

