#pragma once

#ifdef  PLS_OS_WIN

#	pragma warning( disable : 4820 )  // Alignment
#	pragma warning( disable : 4996 )  // Unsafe String Functions
#	pragma warning( disable : 4065 )  // Switch has no case
#	pragma warning( disable : 4355 )  // this pointer used in basic member initializer
#	pragma warning( disable : 4625 )  // copy contructor has been removed implicitly
#	pragma warning( disable : 4626 )  // assign contructor has been removed implicitly
#	pragma warning( disable : 4061 )  // not processed switch item
#	pragma warning( disable : 4711 )  // ... automatic inline extension
#	pragma warning( disable : 4710 )  // ... automatic inline extension
#	pragma warning( disable : 4577 )  // noexcept


#   define __builtin_FILE()         nullptr
#   define __builtin_LINE()         0
#   define __builtin_FUNCTION()     nullptr

#endif//PLS_OS_WIN
