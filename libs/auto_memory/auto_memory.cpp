#include <iostream>
#include <stdlib.h>

#define DEFINE_DEBUG_NEW 0
#include "auto_memory.h"
#undef  DEFINE_DEBUG_NEW


/*
 * statics
 */
void* auto_trace_alloc( size_t size, const char* file, const int line, const char* func );
void auto_trace_free( void* ptr );


/*
 * interfaces
 */
void* operator new( size_t size, const char* file, const int line, const char* func )
{
	return auto_trace_alloc( size, file, line, func );
}

void* operator new[]( size_t size, const char* file, const int line, const char* func )
{
	return auto_trace_alloc( size, file, line, func );
}

void operator delete( void* ptr ) noexcept
{
	auto_trace_free( ptr );
}

void operator delete[]( void* ptr ) noexcept
{
	auto_trace_free( ptr );
}

bool delete_marker( const char* file, const int line, const char* func )
{

	printf( "[MemFree ] file: %s, line: %d, func: %s, size: ??, addr: %p\n", file, line, func, nullptr );
	//
	// TODO push position info to tls
	//

	return true;
}


/*
 * statics
 */
void* auto_trace_alloc( size_t size, const char* file, const int line, const char* func )
{
	void* ptr = malloc( size );
	if( ptr == NULL )
	{
		throw std::bad_alloc();
	}
	printf( "[MemAlloc] file: %s, line: %d, func: %s, size: %lu, addr: %p\n", file, line, func, size, ptr );
	return ptr;
}

void auto_trace_free( void* ptr )
{
	const char* file = nullptr;
	const int line = 0;
	const char* func = nullptr;

	//
	// TODO
	//

	printf( "[MemFree ] file: %s, line: %d, func: %s, size: ??, addr: %p\n", file, line, func, ptr );
}

