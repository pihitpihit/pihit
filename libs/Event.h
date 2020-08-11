#pragma once

#include "Lock.h"

enum class EventFlag
{
	NONE,
	TIMEOUT,
	SIGNALED,
	ABANDONED,
};

class Event
{
public:
	static const int INFINITE = -1;

	public:
		Event();
		~Event();

	public:
		void Signal( bool bBroadcast = false );

		template <typename T>
		void Signal( T eCustomFlag,
					 bool bBroadcast = false );

		EventFlag Wait( int nTimeout = Event::INFINITE,
						bool bClearOnWait = false,
						bool bClearOnWakeUp = true );

		template <typename T>
		EventFlag Wait( T & eCustomFlag,
						int nTimeout = Event::INFINITE,
						bool bClearOnWait = false,
						bool bClearOnWakeUp = true );

		EventFlag Signaled();

		void Clear();

	private:
		//ConditionLock cCondtion_;
		EventFlag     eFlag_;
		int           nCustomFlag_;
};

