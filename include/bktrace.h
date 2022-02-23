// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2006 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "ulimits.h"

namespace ustl {

class ostringstream;
class istream;
class ostream;

/// \class CBacktrace bktrace.h ustl.h
///
/// \brief Stores the backtrace from the point of construction.
///
/// The backtrace, or callstack, is the listing of functions called to
/// reach the construction of this object. This is useful for debugging,
/// to print the location of an error. To get meaningful output you'll
/// need to use a debug build with symbols and with frame pointers. For
/// GNU ld you will also need to link with the -rdynamic option to see
/// actual function names instead of __gxx_personality0+0xF4800.
///
class CBacktrace {
public:
			CBacktrace (void) noexcept;
			CBacktrace (const CBacktrace& v) noexcept;
    inline		~CBacktrace (void) noexcept	{ if (_symbols) free (_symbols); }
    const CBacktrace&	operator= (const CBacktrace& v) noexcept;
    void		text_write (ostringstream& os) const;
    void		read (istream& is);
    void		write (ostream& os) const;
    size_t		stream_size (void) const;
private:
    void		GetSymbols (void) noexcept;
private:
    void*		_addresses [64];	///< Addresses of each function on the stack.
    char*		_symbols;		///< Symbols corresponding to each address.
    uint32_t		_nFrames;		///< Number of addresses in _addresses.
    uint32_t		_symbolsSize;		///< Size of _symbols.
};

} // namespace ustl
