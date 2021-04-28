#pragma once
#include <map>
#include <auto_thread.h>

namespace Plastics
{
	template <typename T>
	class AutoTls
	{
		public:
			AutoTls();
			AutoTls( T init );
			~AutoTls();

		public:
			T& Get();
			void Set( T& newValue );
			T& operator()();
			void operator()( T& newValue );

		private:
			const T init_;
			std::map<ThreadId, T> storage_;
	};

	template <typename T>
	AutoTls<T>::AutoTls():
		init_(),
		storage_()
	{
	}

	template <typename T>
	AutoTls<T>::AutoTls( T init ):
		init_( init ),
		storage_()
	{
	}

	template <typename T>
	AutoTls<T>::~AutoTls()
	{
	}

	template <typename T>
	T& AutoTls<T>::Get()
	{
		ThreadId tid = AutoThread::GetTid();

		if(	storage_.find( tid ) == storage_.end() )
		{
			storage_.insert( make_pair( tid, T() ) );
		}

		return storage_[tid];
	}

	template <typename T>
	void AutoTls<T>::Set( T& newValue )
	{
		ThreadId tid = AutoThread::GetTid();
		storage_.insert( make_pair( tid, newValue ) );
	}

	template <typename T>
	T& AutoTls<T>::operator()()
	{
		return Get();
	}

	template <typename T>
	void AutoTls<T>::operator()( T& newValue )
	{
		Set( newValue );
	}
};

