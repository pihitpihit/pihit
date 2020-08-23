#include <stdarg.h>
#include <string.h>
#include <auto_log.h>
#include <auto_util.h>

using namespace Plastics;

AutoFinalize( AutoLog::Finalize );

static const char* gs_logLevelSymbol[] = {
	"AUTO",
	"TRACE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"CRIT",
};

bool AutoLog::initialized_ = false;
bool AutoLog::basename_ = true;
FILE* AutoLog::logFile_ = stdout;
Lock AutoLog::lock_;

AutoLog::AutoLog( const char* file,
				  const int line,
				  const char* func ):
	file_( file ),
	line_( line ),
	func_( func )
{
	base_ = strrchr( file_, '/' );
	if( base_ )
	{
		base_++;
	}

	const char* name = ( AutoLog::basename_ ? base_ : file_ );
	AutoLog::Log( LogLevel::AUTO, "%s(%d):%s -> Enter", name, line_, func_ );
}

AutoLog::~AutoLog()
{
	const char* name = ( AutoLog::basename_ ? base_ : file_ );
	char liner[8] = "-------";
	liner[ Util::Log( 10, line_ ) ] = '\0';
	AutoLog::Log( LogLevel::AUTO, "%s(%s):%s -> Leave", name, liner, func_ );
}

result_t AutoLog::Initialize()
{
	CriticalSection cs( AutoLog::lock_ );
	{
		// TODO: Initialize
		return Result::Success;
	}
}

void AutoLog::Finalize()
{
	CriticalSection cs( AutoLog::lock_ );
	{
		// TODO Finalize
	}
}

static const char* gs_logPrefixMultiLine = "                    ";
static const char* gs_logSymbolMidLine   = "     |-";
static const char* gs_logSymbolLastLine  = "     +-";

void AutoLog::Log( LogLevel level, const char* const format, ... )
{
	// TODO: make thread safe

	// timestamp
	struct tm ptm;
	struct timespec now;
	char timestr[100];

	::clock_gettime(CLOCK_REALTIME, &now);
	localtime_r(&now.tv_sec, &ptm);
	strftime( timestr, sizeof( timestr ), "%m-%d %T", &ptm );

	// make contents
	char buf[2048];
	va_list ap;
	va_start( ap, format );
	{
		vsnprintf( buf, 2048, format, ap );
		buf[2047] = '\0';
	}
	va_end( ap );

	// split lines
	std::vector<char*> lines;
	char* line = strtok( buf, "\n" );
	while( line )
	{
		lines.push_back( line );
		line = strtok( nullptr, "\n" );
	}

	CriticalSection cs( AutoLog::lock_ );
	{
		fprintf( logFile_,
				 "[%s.%03ld][%-5s] ",
				 timestr,
				 now.tv_nsec / 1000000,
				 gs_logLevelSymbol[(int)level] );

		// head line
		fprintf( logFile_, "%s\n", lines[0] );

		// mid line
		for( int i = 1; i < (int)lines.size() - 1; i++ )
		{
			fprintf( logFile_, "%s%-5s %s\n",
					 gs_logPrefixMultiLine,
					 gs_logSymbolMidLine,
					 lines[i] );
		}

		// last line
		if( 2 <= lines.size() )
		{
			fprintf( logFile_, "%s%-5s %s\n",
					 gs_logPrefixMultiLine,
					 gs_logSymbolLastLine,
					 lines[lines.size()-1] );
		}
	}
}

