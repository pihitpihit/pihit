#pragma once

#include <auto_result.h>
#include <auto_sync.h>

namespace Plastics
{
	enum class LogLevel : int
	{
		Auto,
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Crit,
		/* TODO: add here. */
		Count
	};

	class AutoLog
	{
		public:
			AutoLog( const char* file = __builtin_FILE(),
					 const int   line = __builtin_LINE(),
					 const char* func = __builtin_FUNCTION() );
			~AutoLog();

		public:
			static result_t Initialize();
			static void Finalize();
			static void Log( LogLevel level,
							 const char* const format, ... );

		private:
			const char* file_;
			const int   line_;
			const char* func_;
			const char* base_;

		private:
			static bool initialized_;
			static bool basename_;
			static FILE* logFile_;
			static Lock lock_;
	};

};

