#include <iostream>
#include <stdlib.h>

#define DEFINE_DEBUG_NEW 0
#include <auto_memory.h>
#undef  DEFINE_DEBUG_NEW

#include <auto_tls.hpp>

using namespace Plastics;


/*
 * internals
 */
class MemLogRecord
{
	public:
		MemLogRecord():
			file_( nullptr ), line_( -1 ), func_( nullptr ), size_( -1 )
		{
		}
		MemLogRecord( const char* file, const int line, const char* func, const int size = -1 ):
			file_( file ), line_( line ), func_( func ), size_( size )
		{
		}

	public:
		const char* file_;
		int         line_;
		const char* func_;
		int         size_;
};

class MemLog: public std::map<void*, MemLogRecord>
{
	public:
		~MemLog()
		{
			int nLeakCount = (int)size();
			if( nLeakCount )
			{
				printf( "[MemLog] remain leaked count : %d\n", nLeakCount );
			}
		}

		void MarkForDelete( MemLogRecord record )
		{
			lastMarkForDelete_ = record;
			bMarkedForDelete_ = true;
		}
		MemLogRecord& GetMark()
		{
			return lastMarkForDelete_;
		}
		void Delete( void* ptr )
		{
			if( find( ptr ) != end() )
			{
				auto& rec = (*this)[ptr];
				auto& mark = GetMark();

				printf( "[MemFree ] file: %s, line: %d, func: %s, size: %d, addr: %p\n", mark.file_, mark.line_, mark.func_, rec.size_, ptr );
				erase( ptr );
				bMarkedForDelete_ = false;
			}

		}

	private:
		bool bMarkedForDelete_;
		MemLogRecord lastMarkForDelete_;
};

/*
 *  globals
 */
static AutoTls<MemLog> g_tlsMemLog;

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

bool AutoMemory::delete_marker( const char* file, const int line, const char* func )
{
	g_tlsMemLog().MarkForDelete( MemLogRecord( file, line, func ) );
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
	g_tlsMemLog().insert( std::make_pair( ptr, MemLogRecord( file, line, func, size ) ) );
	return ptr;
}

void auto_trace_free( void* ptr )
{
	g_tlsMemLog().Delete( ptr );
	free( ptr );
}

