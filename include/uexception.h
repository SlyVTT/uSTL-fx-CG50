// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "utypes.h"
#include "bktrace.h"

namespace std {
/// If you write a replacement terminate handler, it must be of this type.
using terminate_handler = void (*)(void);
/// Takes a new handler function as an argument, returns the old function.
terminate_handler set_terminate (terminate_handler pHandler) noexcept;
/// The runtime will call this function if exception handling must be
/// abandoned for any reason.  It can also be called by the user.
void terminate (void) noexcept __attribute__ ((__noreturn__));
/// The runtime will call this function if an exception is thrown which
/// violates the function's exception specification.
void unexpected (void) __attribute__ ((__noreturn__));
/// Returns true when the caught exception violates the throw specification.
bool uncaught_exception() noexcept;
} // namespace std

namespace ustl {

class string;

using xfmt_t = uint32_t;

enum {
    xfmt_Exception,
    xfmt_BadAlloc
};

/// \class exception uexception.h ustl.h
/// \ingroup Exceptions
///
/// \brief Base class for exceptions, equivalent to std::exception.
///
class exception {
public:
    using rcbktrace_t = const CBacktrace&;
public:
    inline		exception (void) noexcept : _backtrace(),_format (xfmt_Exception) {}
    inline virtual	~exception (void) noexcept {}
    inline virtual const char* what (void) const noexcept { return "error"; }
    virtual void	info (string& msgbuf, const char* fmt = nullptr) const noexcept;
    virtual void	read (istream& is);
    virtual void	write (ostream& os) const;
    void		text_write (ostringstream& os) const noexcept;
    inline virtual size_t stream_size (void) const noexcept { return sizeof(_format) + sizeof(uint32_t) + _backtrace.stream_size(); }
    /// Format of the exception is used to lookup exception::info format string.
    /// Another common use is the instantiation of serialized exceptions, used
    /// by the error handler node chain to troubleshoot specific errors.
    inline xfmt_t	format (void) const	{ return _format; }
    inline rcbktrace_t	backtrace (void) const	{ return _backtrace; }
protected:
    inline void		set_format (xfmt_t fmt) { _format = fmt; }
private:
    CBacktrace		_backtrace;	///< Backtrace of the throw point.
    xfmt_t		_format;	///< Format of the exception's data.
};

/// \class bad_cast uexception.h ustl.h
/// \ingroup Exceptions
///
/// \brief Thrown to indicate a bad dynamic_cast usage.
///
class bad_cast : public exception {
public:
    inline 			bad_cast (void) noexcept		: exception() {}
    inline virtual const char*	what (void) const noexcept override	{ return "bad cast"; }
};

class bad_typeid : public exception {
public:
    inline			bad_typeid (void) noexcept		{ }
    inline virtual const char*	what (void) const noexcept override	{ return "bad typeid"; }
};

const char* demangle_type_name (char* buf, size_t bufSize, size_t* pdmSize = nullptr) noexcept;

//----------------------------------------------------------------------

/// \class bad_alloc uexception.h ustl.h
/// \ingroup Exceptions
///
/// \brief Exception thrown on memory allocation failure by memblock::reserve.
///
} // namespace ustl
namespace std {
class bad_alloc : public ::ustl::exception {
public:
    explicit		bad_alloc (size_t nBytes = 0) noexcept;
    inline virtual const char*	what (void) const noexcept override { return "memory allocation failed"; }
    virtual void	info (ustl::string& msgbuf, const char* fmt = nullptr) const noexcept override;
    virtual void	read (ustl::istream& is) override;
    virtual void	write (ustl::ostream& os) const override;
    virtual size_t	stream_size (void) const noexcept override;
protected:
    size_t		_bytesRequested;	///< Number of bytes requested by the failed allocation.
};

} // namespace std
namespace ustl {
    using bad_alloc = std::bad_alloc;
} // namespace ustl
