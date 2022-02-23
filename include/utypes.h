// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#define __STDC_LIMIT_MACROS	// For WCHAR_MIN and WCHAR_MAX in stdint.
#define __STDC_CONSTANT_MACROS	// For UINT??_C macros to avoid using L and UL suffixes on constants.
#include "config.h"
#if __has_include(<stdint.h>)
    #include <stdint.h>
#elif __has_include(<inttypes.h>)
    #include <inttypes.h>
#else
    #error "Need standard integer types definitions, usually in stdint.h"
#endif
#if __has_include(<sys/types.h>)
    #include <sys/types.h>
#endif
#include <stddef.h>		// For ptrdiff_t, size_t
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#if __has_include(<alloca.h>)
    #include <alloca.h>
#endif
#ifndef SIZE_MAX
    #define SIZE_MAX		UINT_MAX
#endif
#if sun || __sun		// Solaris defines UINTPTR_MAX as empty.
    #undef UINTPTR_MAX
    #define UINTPTR_MAX		ULONG_MAX
#endif
#ifndef WCHAR_MAX
    #ifdef __WCHAR_MAX__
	#define WCHAR_MAX	__WCHAR_MAX__
    #else
	#define WCHAR_MAX	CHAR_MAX
    #endif
#endif
#ifndef LLONG_MAX
    #define ULLONG_MAX		UINT64_C(0xFFFFFFFFFFFFFFFF)
    #define LLONG_MAX		INT64_C(0x7FFFFFFFFFFFFFFF)
    #define LLONG_MIN		ULLONG_MAX
#endif
#ifndef BYTE_ORDER
    #define LITTLE_ENDIAN	USTL_LITTLE_ENDIAN
    #define BIG_ENDIAN		USTL_BIG_ENDIAN
    #define BYTE_ORDER		USTL_BYTE_ORDER
#endif

using uoff_t = size_t;		///< A type for storing offsets into blocks measured by size_t.
using hashvalue_t = uint32_t;	///< Value type returned by the hash functions.
using streamsize = size_t;	///< Size of stream data
using streamoff = uoff_t;	///< Offset into a stream

#if !defined(UINTPTR_MAX) || !defined(UINT32_C)
    #error "If you include stdint.h before ustl.h, define __STDC_LIMIT_MACROS and __STDC_CONSTANT_MACROS first"
#endif
#if WANT_ALWAYS_INLINE
    #define inline INLINE inline
#endif
