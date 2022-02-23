// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "memlink.h"
#include "uexception.h"
#include "utf8.h"
#include "uios.h"
#include "strmsize.h"
#if WANT_BSTREAM_EXCEPTIONS
    #include "typeinfo.h"
#endif

namespace ustl {

class istream;
class string;

/// \class ostream mostream.h ustl.h
/// \ingroup BinaryStreams
///
/// \brief Helper class to write packed binary streams.
///
/// This class contains a set of functions to write integral types into an
/// unstructured memory block. Packing binary file data can be done this
/// way, for instance. aligning the data is your responsibility, and can
/// be accomplished by proper ordering of writes and by calling align.
/// Unaligned access is usually slower by orders of magnitude and,
/// on some architectures, such as PowerPC, can cause your program to crash.
/// Therefore, all write functions have asserts to check alignment.
/// See \ref istream documentation for rules on designing your data format.
/// Overwriting the end of the stream will also cause a crash (an assert in
/// debug builds). Oh, and don't be intimidated by the size of the inlines
/// here. In the assembly code the compiler will usually chop everything down
/// to five instructions each.
///
/// Example code:
/// \code
///     memblock b;
///     ostream os (b);
///     os << boolVar << ios::talign<int>();
///     os << intVar << floatVar;
///     os.write (binaryData, binaryDataSize);
///     os.align();
///     b.resize (os.pos());
///     b.write_file ("test.file");
/// \endcode
///
class ostream : public memlink, public ios_base {
public:
    constexpr		ostream (void)			: memlink(), _pos(0) {}
    constexpr		ostream (void* p, streamsize n)	: memlink (p, n), _pos (0) {}
    constexpr explicit	ostream (const memlink& source)	: memlink (source), _pos (0) {}
    constexpr iterator	end (void)			{ return memlink::end(); }
    constexpr const_iterator end (void) const		{ return memlink::end(); }
    inline void		seek (uoff_t newPos);
    inline void		iseek (const_iterator newPos);
    inline void		skip (streamsize nBytes);
    constexpr uoff_t	pos (void) const		{ return _pos; }
    constexpr iterator	ipos (void)			{ return begin() + pos(); }
    constexpr const_iterator ipos (void) const		{ return begin() + pos(); }
    constexpr streamsize remaining (void) const		{ return size() - pos(); }
    constexpr bool	aligned (streamsize grain = c_DefaultAlignment) const
			    { return pos() % grain == 0; }
    constexpr streamsize align_size (streamsize grain = c_DefaultAlignment) const
			    { return Align (pos(), grain) - pos(); }
    bool		verify_remaining (const char* op, const char* type, size_t n);
    void		align (streamsize grain = c_DefaultAlignment);
    inline void		write (const void* buffer, streamsize size);
    inline void		write (const cmemlink& buf);
    void		write_strz (const char* str);
    void		read (istream& is);
    inline void		write (ostream& os) const	{ os.write (begin(), pos()); }
    void		text_write (ostringstream& os) const;
    constexpr size_t	stream_size (void) const	{ return pos(); }
    void		insert (iterator start, streamsize size);
    void		erase (iterator start, streamsize size);
    constexpr void	swap (ostream& os)		{ memlink::swap (os); ::ustl::swap (_pos, os._pos); }
    template <typename T>
    inline void		iwrite (const T& v);
    virtual ostream&	flush (void)			{ return *this; }
    virtual streamsize	overflow (streamsize=1)		{ return remaining(); }
    virtual void	unlink (void) noexcept override;
    inline void		link (void* p, streamsize n)	{ memlink::link (p, n); }
    inline void		link (memlink& l)		{ memlink::link (l.data(), l.writable_size()); }
    inline void		link (void* f, void* l)		{ memlink::link (f, l); }
    constexpr void	relink (void* p, streamsize n)	{ memlink::relink (p, n); _pos = 0; }
    constexpr void	relink (memlink& l)		{ relink (l.data(), l.writable_size()); }
    inline void		seekp (off_t p, seekdir d = beg);
    constexpr off_t	tellp (void) const		{ return pos(); }
protected:
    constexpr void	SetPos (uoff_t newPos)		{ _pos = newPos; }
private:
    streamoff		_pos;	///< Current write position.
};

//----------------------------------------------------------------------

/// \class ostream_iterator mostream.h ustl.h
/// \ingroup BinaryStreamIterators
///
/// \brief An iterator over an ostream to use with uSTL algorithms.
///
template <typename T, typename Stream = ostream>
class ostream_iterator {
public:
    using value_type		= T;
    using difference_type	= ptrdiff_t;
    using pointer		= value_type*;
    using reference		= value_type&;
    using size_type		= typename Stream::size_type;
    using iterator_category	= output_iterator_tag;
public:
    constexpr explicit		ostream_iterator (Stream& os)
				    : _os (os) {}
    constexpr			ostream_iterator (const ostream_iterator& iter)
				    : _os (iter._os) {}
    /// Writes \p v into the stream.
    inline ostream_iterator&	operator= (const T& v)
				    { _os << v; return *this; }
    constexpr ostream_iterator&	operator* (void) { return *this; }
    constexpr ostream_iterator&	operator++ (void) { return *this; }
    constexpr ostream_iterator	operator++ (int) { return *this; }
    inline ostream_iterator&	operator+= (streamsize n) { _os.skip (n); return *this; }
    constexpr bool		operator== (const ostream_iterator& i) const
				    { return _os.pos() == i._os.pos(); }
    constexpr bool		operator< (const ostream_iterator& i) const
				    { return _os.pos() < i._os.pos(); }
private:
    Stream&	_os;
};

//----------------------------------------------------------------------

using ostream_iterator_for_utf8 = ostream_iterator<utf8subchar_t>;
using utf8ostream_iterator = utf8out_iterator<ostream_iterator_for_utf8>;

/// Returns a UTF-8 adaptor writing to \p os.
inline utf8ostream_iterator utf8out (ostream& os)
{
    ostream_iterator_for_utf8 si (os);
    return utf8ostream_iterator (si);
}

//----------------------------------------------------------------------

/// Checks that \p n bytes are available in the stream, or else throws.
inline bool ostream::verify_remaining (const char* op, const char* type, size_t n)
{
    const size_t rem = remaining();
    bool enough = n <= rem;
    if (!enough) overrun (op, type, n, pos(), rem);
    return enough;
}

/// Move the write pointer to \p newPos
inline void ostream::seek (uoff_t newPos)
{
#if WANT_BSTREAM_EXCEPTIONS
    if (newPos > size())
	return overrun ("seekp", "byte", newPos, pos(), size());
#else
    assert (newPos <= size());
#endif
    SetPos (newPos);
}

/// Sets the current write position to \p newPos
inline void ostream::iseek (const_iterator newPos)
{
    seek (distance (begin(), const_cast<iterator>(newPos)));
}

/// Sets the current write position to \p p based on \p d.
inline void ostream::seekp (off_t p, seekdir d)
{
    switch (d) {
	case beg:	seek (p); break;
	case cur:	seek (pos() + p); break;
	case ios_base::end:	seek (size() - p); break;
    }
}

/// Skips \p nBytes without writing anything.
inline void ostream::skip (streamsize nBytes)
{
    seek (pos() + nBytes);
}

/// Writes \p n bytes from \p buffer.
inline void ostream::write (const void* buffer, size_type n)
{
#if WANT_BSTREAM_EXCEPTIONS
    if (!verify_remaining ("write", "binary data", n))
	return;
#else
    assert (remaining() >= n && "Buffer overrun. Check your stream size calculations.");
#endif
    memcpy (ipos(), const_iterator(buffer), n);
    _pos += n;
}

/// Writes the contents of \p buf into the stream as a raw dump.
inline void ostream::write (const cmemlink& buf)
{
    write (buf.begin(), buf.size());
}

/// Writes type T into the stream via a direct pointer cast.
template <typename T>
inline void ostream::iwrite (const T& v)
{
    assert (aligned (stream_align_of (v)));
#if WANT_BSTREAM_EXCEPTIONS
    if (!verify_remaining ("write", typeid(v).name(), sizeof(T)))
	return;
#else
    assert (remaining() >= sizeof(T));
#endif
    *reinterpret_cast<T*>(ipos()) = v;
    SetPos (pos() + sizeof(T));
}

//----------------------------------------------------------------------

template <typename T> struct object_writer {
    inline void operator()(ostream& os, const T& v) const { v.write (os); }
};
template <typename T> struct integral_object_writer {
    inline void operator()(ostream& os, const T& v) const { os.iwrite (v); }
};
template <typename T>
inline ostream& operator<< (ostream& os, const T& v) {
    using object_writer_t = typename tm::Select <numeric_limits<T>::is_integral,
	integral_object_writer<T>, object_writer<T> >::Result;
    object_writer_t()(os, v);
    return os;
}

//----------------------------------------------------------------------

} // namespace ustl
