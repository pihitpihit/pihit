#pragma once

void* operator new( size_t size, const char* file, const int line, const char* func );
void* operator new[]( size_t size, const char* file, const int line, const char* func );
void operator delete( void* ptr ) noexcept;
void operator delete[]( void* ptr ) noexcept;

namespace Plastics
{
	class AutoMemory
	{
		public:
			static bool delete_marker( const char* file, const int line, const char* func );
	};
};

#ifndef DEFINE_DEBUG_NEW
#define DEFINE_DEBUG_NEW 1
#endif//DEFINE_DEBUG_NEW

#if DEFINE_DEBUG_NEW == 1
#	define DEBUG_NEW new( __FILE__, __LINE__, __func__ )
#	define new DEBUG_NEW
#	define DEBUG_DEL AutoMemory::delete_marker( __FILE__, __LINE__, __func__ )
#	define delete if( DEBUG_DEL ) delete
#endif

