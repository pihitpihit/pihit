
#include <cassert>
#include <cxxabi.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <errno.h>
#include "auto_result.h"

#define REPORT_NONE			( 0x00000000 )
#define REPORT_ON_ASSIGN	( 0x00000001 )
#define REPORT_ON_FINAL		( 0x00000002 )
#define REPORT_BY_THROW		( 0x00000004 )

const char* exception_t::name() const
{
	assert( !"Cannot use this function(exception_t)" );
	return nullptr;
}
const char* exception_t::desc() const
{
	assert( !"Cannot use this function(exception_t)" );
	return nullptr;
}
const char* exception_t::what() const noexcept
{
	assert( !"Cannot use this function(exception_t)" );
	return nullptr;
}

char* demangle( const char* mangledName )
{
	size_t len;
	int status;
	return abi::__cxa_demangle( mangledName, NULL, &len, &status );
}
result_t::result_t():
	line_( 0 )
{
}
result_t::result_t( const result_t& result, AUTO_POSITION_IN ):
	result_( result.result_ ),
	type_( REPORT_NONE ),
	logs_( nullptr ),
	file_( file ),
	line_( line ),
	func_( func )
{
}
result_t::result_t( const result_code& result, AUTO_POSITION_IN ):
	result_( &result ),
	type_( REPORT_NONE ),
	logs_( nullptr ),
	file_( file ),
	line_( line ),
	func_( func )
{
}
result_t::result_t( const result_record& result, AUTO_POSITION_IN ):
	result_( result.result_ ),
	type_( result.typeOn_ ),
	logs_( nullptr ),
	file_( file ),
	line_( line ),
	func_( func )
{
	if( isType( REPORT_ON_FINAL ) )
	{
		initLog();
	}
}
result_t::result_t( const int error ):
	result_t( ErrorToResult( error ) )
{
}
result_t::~result_t()
{
	if( isType( REPORT_ON_FINAL ) )
	{
		printf( "[----][ErrorReport] Start.\n" );
		printf( "[----] Declared at %s(%d):%s()\n", file_, line_, func_ );
		if( logs_->size() )
		{
			printf( "[----] There were %lu errors.\n", logs_->size() );
			int idx = 0;
			for( auto r : *logs_ )
			{
				printf( "[  %02d] %s: line %d: function %s: ErrorOccurred(%s)\n",
						idx, r.file_, r.line_, r.func_, r.result_->name() );
				idx++;
			}
			printf( "[----][ErrorReport] End.\n" );
		}
		else
		{
			printf( "[----] There wrer no error.\n" );
		}

	}
	if( logs_ )
	{
		delete logs_;
	}
	if( isType( REPORT_ON_FINAL|REPORT_BY_THROW ) )
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wexceptions"
		throw *this;
#pragma GCC diagnostic pop
	}
}
uint32_t result_t::id() const
{
	return result_->getId();
}
result_t::operator int() const
{
	return result_->getId();
}
result_t& result_t::operator=( const result_t& result )
{
	result_ = result.result_;
	return *this;
}
result_t& result_t::operator=( const result_code& result )
{
	result_ = &result;
	return *this;
}
result_t& result_t::operator=( const result_record& result )
{
	result_ = result.result_;
	type_ |= result.typeOn_;
	type_ &= ~result.typeOff_;

	if( isType( REPORT_ON_FINAL ) )
	{
		initLog();
		logs_->push_back( result );
	}
	if( isType( REPORT_ON_ASSIGN ) )
	{
		printf( "[----] %s(%d):%s:Error Assigned(%s)\n",
			result.file_, result.line_,
			result.func_, result.result_->name() );
		if( isType( REPORT_BY_THROW ) )
		{
			throw *this;
		}
	}

	return *this;
}
result_t& result_t::operator=( const int error )
{
	return operator=( ErrorToResult( error ) );
}
const char* result_t::name() const
{
	return result_->getName();
}
const char* result_t::desc() const
{
	return result_->getDesc();
}
const char* result_t::what() const noexcept
{
	return result_->getDesc();
}
bool result_t::isType( uint32_t type )
{
	return ( type_ & type ) == type;
}
void result_t::backtrace( AUTO_POSITION_IN )
{
	void* array[10];
	size_t size;

	size = ::backtrace( array, 10 );
	printf( "backtrack returns size(%d)\n", (int)size );

	//backtrace_symbols_fd( &array[1], size - 1, STDERR_FILENO );
	//backtrace_symbols_fd( array, size, STDERR_FILENO );
	char** frames = backtrace_symbols( array, size );

	printf( "++++++++++++++++++++++++++++++\n" );
	for( int i = 0; i < size; i++ )
	{
		printf( "%s\n", frames[i] );
	}
	printf( "++++++++++++++++++++++++++++++\n" );
}
void result_t::initLog()
{
	if( !logs_ )
	{
		logs_ = new std::vector<result_record>();
	}
}


result_code::result_code():
	result_( this )
{
}
result_code::~result_code()
{
}
const char* result_code::name() const
{
	return result_->getName();
}
const char* result_code::desc() const
{
	return result_->getDesc();
}
const char* result_code::what() const noexcept
{
	return result_->getDesc();
}
uint32_t result_code::getId() const
{
	assert( !"Cannot use this function" );
	return 0;
}
const char* result_code::getName() const
{
	assert( !"Cannot use this function" );
	return nullptr;
}
const char* result_code::getDesc() const
{
	assert( !"Cannot use this function" );
	return nullptr;
}
result_record result_code::operator()( const char* msg, AUTO_POSITION_IN ) const
{
	return result_record( *this, msg, AUTO_POSITION_PASS );
}



result_record::result_record( const result_code& result, const char* msg, AUTO_POSITION_IN ):
	result_( &result ),
	typeOn_( REPORT_NONE ),
	typeOff_( REPORT_NONE ),
	msg_( nullptr ),
	file_( file ),
	line_( line ),
	func_( func )
{
	if( msg )
	{
		msg_ = strdup( msg );
		if( msg_ == NULL )
		{
			throw std::bad_alloc();
		}
	}
}
result_record::~result_record()
{
	if( msg_ ) free( (void*)msg_ );
}
const char* result_record::name() const
{
	return result_->name();
}
const char* result_record::desc() const
{
	return result_->desc();
}
const char* result_record::what() const noexcept
{
	if( msg_ ) return msg_;
	return result_->desc();
}
result_record& result_record::addType( uint32_t type )
{
	typeOn_ |= type;
	typeOff_ &= ~type;
	assert( ( typeOn_ & typeOff_ ) == REPORT_NONE );
	return *this;
}
result_record& result_record::subType( uint32_t type )
{
	typeOff_ |= type;
	typeOn_ &= ~type;
	assert( ( typeOn_ & typeOff_ ) == REPORT_NONE );
	return *this;
}
result_record& result_record::onAssign()
{
	subType( REPORT_ON_FINAL );
	addType( REPORT_ON_ASSIGN );
	return *this;
}
result_record& result_record::onFinal()
{
	subType( REPORT_ON_ASSIGN );
	addType( REPORT_ON_FINAL );
	return *this;
}
result_record& result_record::byThrow()
{
	addType( REPORT_BY_THROW );
	return *this;
}

result_t ErrorToResult( const int error )
{
	switch( error )
	{
	case ETIMEDOUT:
		return Result::Timeout;
	default:
		return Result::Internal;
	}
}

