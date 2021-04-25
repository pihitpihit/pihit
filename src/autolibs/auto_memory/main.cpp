#include <iostream>
#include <stdlib.h>
#ifndef PLS_OS_WIN
#	include <pthread.h>
#endif//PLS_OS_WIN

#include <auto_memory.h>
#include <auto_windows.h>

using namespace Plastics;


class testg
{
	public:
		testg(){printf("testg created\n");}
		~testg(){printf("testg destroyed\n");}
		//void pp(){printf("pp\n");}
	public:
		int a;
};

class testg2
{
	public:
		testg2(){printf("testg2 created\n");}
		~testg2(){printf("testg2 destroyed\n");}
		//void pp(){printf("pp22\n");}
	public:
		int a;
};

thread_local testg2 aaa;

static testg test;

//void* foo( void* arg )
//{
//	testg* ptest = new testg;
//	delete ptest;
//	printf( "before aaa.ppp in thread...........\n" );
//	aaa.pp();
//	return nullptr;
//}

int main( void )
{
	int* pVal = new int;
	//test.pp();
	printf( "%p\n", pVal );

	testg* ptest = new testg;

	delete ptest;

	//aaa.pp();
	//pthread_t th;
	//int status;
	//pthread_create( &th, NULL, foo, NULL);
	//pthread_join( th, (void**)&status );
	//printf( "before process return.......\n" );
	return 0;
}
