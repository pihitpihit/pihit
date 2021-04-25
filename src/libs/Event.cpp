#include "Event.h"

Event::Event()
{
}

Event::~Event()
{
}

void Event::Signal( bool bBroadcast )
{
}

template <typename T>
void Event::Signal( T eCustomFlag, bool bBroadcast )
{
}

EventFlag Event::Wait( int nTimeout, bool bClearOnWait, bool bClearOnWakeUp )
{
	return eFlag_;
}

template <typename T>
EventFlag Event::Wait( T & eCustomFlag, int nTimeout, bool bClearOnWait, bool bClearOnWakeUp )
{
	return eFlag_;
}

EventFlag Event::Signaled()
{
	return eFlag_;
}

void Event::Clear()
{
}

