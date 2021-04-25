
#include <iostream>
#include <auto_result.h>

using namespace Plastics;

void foo()
{
	result_t result = Result::Internal;
	throw result;
	//throw Result::MemoryAllocFail;
}

int main( void )
{
	result_t r = Result::Success().onFinal();


	r = Result::Internal();

	r = Result::Internal();
	r = Result::Internal();
	r = Result::Internal();
	r = Result::Internal();



	r = Result::InvalidParameter();
	return 0;
	/*
	return 0;
	printf( "ResultCode::Success = %d\n", (int)ResultCode::Success );

	//result_t result = Result::Success().onFinal().throwOnReport();
	result_t result = Result::Success;

	printf( "result = %d\n", result.id() );
	printf( "sizeof(result_t) = %zu\n", sizeof( result_t ) );
	printf( "sizeof(std::exception) = %zu\n", sizeof( std::exception ) );

	printf( "\n>>>>> test id/name/desc <<<<<\n" );
	result = Result::Internal();
	printf( "result.id   = %d\n", result.id() );
	printf( "result.name = %s\n", result.name() );
	printf( "result.desc = %s\n", result.desc() );

	printf( "\n>>>>> test if-else <<<<<\n" );
	if( result != Result::Success )
	{
		printf( "test if statements ok\n" );
	}
	if( result == Result::Internal )
	{
		printf( "test if statements ok\n" );
	}

	printf( "\n>>>>> test switch-case <<<<<\n" );
	result = Result::InvalidParameter();
	switch( result )
	{
	case Result::Success:
		printf( "test switch-case : Success\n" );
		break;
	case Result::Internal:
		printf( "test switch-case : Internal\n" );
		break;
	case Result::InvalidParameter:
		printf( "test switch-case : InvalidParam\n" );
		break;
	case Result::MemoryAllocFail:
		printf( "test switch-case : MemoryAllocationFail\n" );
		break;
	default:
		printf( "test switch-case : default\n" );
	}


	printf( "\n>>>>> test try-catch <<<<<\n" );
	try
	{
		foo();
	}
	catch( result_t& r )
	{
		printf( "Excpetion Occourred(%s, %s)\n", r.name(), r.desc() );
		printf( "%s\n", r.what() );
		printf( "%s\n", typeid(r).name() );
		printf( "<<%s>>\n", typeid(Result::Success).name() );
		char* name = demangle( typeid(Result::Success).name() );
		printf( "<<%s>>\n", name );
		if( name ) free( name );
	}

	printf( "EndOfMain\n" );

	try
	{
		throw Result::Internal();
	}
	catch( std::exception& e )
	{
		printf( "e.what = %s\n", e.what() );
	}

	return 0;
	*/
}
