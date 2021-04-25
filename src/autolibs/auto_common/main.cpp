#include <auto_log.h>

using namespace Plastics;

void foo();
void foo()
{
	AutoLog a;
}

int main()
{
	AutoLog::Initialize();

	for( int level = 0; level < (int)LogLevel::Count; level++ )
	{
		AutoLog::Log( (LogLevel)level, "aa\na\n%s", "bbb" );
	}

	foo();

	return 0;
}

