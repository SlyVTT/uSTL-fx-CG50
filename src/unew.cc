// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "unew.h"

void* tmalloc (size_t n)
{
    void* p = malloc (n);
    if (!p)
	throw ustl::bad_alloc (n);
    return p;
}

void nfree (void* p) noexcept
    { if (p) free (p); }
extern "C" void nfreen (void* p, size_t) noexcept
    { nfree(p); }

#if __APPLE__	// MacOS lives in the stone age and does not support aliases

void* operator new (size_t n)	{ return tmalloc(n); }
void* operator new[] (size_t n)	{ return tmalloc(n); }

void  operator delete (void* p) noexcept	{ nfree(p); }
void  operator delete[] (void* p) noexcept	{ nfree(p); }
void  operator delete (void* p, size_t sz) noexcept	{ nfreen(p,sz); }
void  operator delete[] (void* p, size_t sz) noexcept	{ nfreen(p,sz); }

#else // !__APPLE__

void* operator new (size_t n)	WEAKALIAS("tmalloc");
void* operator new[] (size_t n)	WEAKALIAS("tmalloc");

void  operator delete (void* p) noexcept	WEAKALIAS("nfree");
void  operator delete[] (void* p) noexcept	WEAKALIAS("nfree");
void  operator delete (void* p, size_t n) noexcept	WEAKALIAS("nfreen");
void  operator delete[] (void* p, size_t n) noexcept	WEAKALIAS("nfreen");

#endif // __APPLE__
