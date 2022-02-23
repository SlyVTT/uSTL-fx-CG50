// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#ifndef NDEBUG	// Optimized code here. asserts slow it down, and are checked elsewhere.
#define NDEBUG
#endif

#include "ualgo.h"

namespace ustl {

//----------------------------------------------------------------------
// rotate_fast

/// Exchanges ranges [first, middle) and [middle, last)
void rotate_fast (void* first, void* middle, void* last) noexcept
{
#if __has_include(<alloca.h>)
    const size_t half1 (distance (first, middle)), half2 (distance (middle, last));
    const size_t hmin (min (half1, half2));
    if (!hmin)
	return;
    void* buf = alloca (hmin);
    if (buf) {
	if (half2 < half1) {
	    copy_n_fast (middle, half2, buf);
	    copy_backward_fast (first, middle, last);
	    copy_n_fast (buf, half2, first);
	} else {
	    copy_n_fast (first, half1, buf);
	    copy_n_fast (middle, half2, first);
	    copy_n_fast (buf, half1, advance_ptr(first,half2));
	}
    } else
#else
    if (first == middle || middle == last)
	return;
#endif
    {
	char* f = static_cast<char*>(first);
	char* m = static_cast<char*>(middle);
	char* l = static_cast<char*>(last);
	reverse (f, m);
	reverse (m, l);
	while (f != m && m != l)
	    iter_swap (f++, --l);
	reverse (f, (f == m ? l : m));
    }
}

//----------------------------------------------------------------------
// Miscellaneous instantiated stuff from headers which don't have enough
// to warrant creation of a separate file.cc
//----------------------------------------------------------------------

// Used in uspecial to print printable characters
const char _FmtPrtChr[2][8]={"'%c'","%d"};

} // namespace ustl
