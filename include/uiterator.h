// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
/// \file uiterator.h
/// \brief Contains various iterator adapters.

#pragma once
#include "utypes.h"

namespace ustl {

//{{{ advance, distance ------------------------------------------------

/// Offsets a pointer
template <typename T>
inline constexpr T* advance_ptr (T* i, ptrdiff_t o)
    { return i+o; }
template <> inline constexpr void* advance_ptr (void* i, ptrdiff_t o)
    { return static_cast<uint8_t*>(i)+o; }
template <> inline constexpr const void* advance_ptr (const void* i, ptrdiff_t o)
    { return static_cast<const uint8_t*>(i)+o; }

/// Offsets an iterator
template <typename T>
inline constexpr T advance (T& i, ptrdiff_t o)
    { return i += o; }
template <> inline constexpr void* advance (void*& i, ptrdiff_t o)
    { return i = advance_ptr (i, o); }
template <> inline constexpr const void* advance (const void*& i, ptrdiff_t o)
    { return i = advance_ptr (i, o); }

/// Returns the difference \p p1 - \p p2
template <typename T1, typename T2>
inline constexpr ptrdiff_t distance (T1 i1, T2 i2)
    { return i2 - i1; }

/// Returns the absolute value of the distance i1 and i2
template <typename T1, typename T2>
inline constexpr size_t abs_distance (T1 i1, T2 i2)
    { return absv (distance(i1, i2)); }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define UNVOID_DISTANCE(T1const,T2const)   \
template <> inline constexpr ptrdiff_t distance (T1const void* p1, T2const void* p2) \
{ return static_cast<T2const uint8_t*>(p2) - static_cast<T1const uint8_t*>(p1); }
UNVOID_DISTANCE(,)
UNVOID_DISTANCE(const,const)
UNVOID_DISTANCE(,const)
UNVOID_DISTANCE(const,)
#undef UNVOID_DISTANCE
#endif

//}}}-------------------------------------------------------------------
//{{{ begin, end, size

// Array range accessors
template <typename T>
inline constexpr auto begin (T& c) -> decltype(c.begin())
    { return c.begin(); }
template <typename T>
inline constexpr auto begin (const T& c) -> decltype(c.begin())
    { return c.begin(); }
template <typename T, size_t N>
inline constexpr T* begin (T (&c)[N]) noexcept
    { return &c[0]; }
template <typename T>
inline constexpr auto cbegin (const T& c) -> decltype(begin(c))
    { return begin(c); }
template <typename T>
inline constexpr auto end (T& c) -> decltype(c.end())
    { return c.end(); }
template <typename T>
inline constexpr auto end (const T& c) -> decltype(c.end())
    { return c.end(); }
template <typename T, size_t N>
inline constexpr T* end (T (&c)[N]) noexcept
    { return &c[N]; }
template <typename T>
inline constexpr auto cend (const T& c) -> decltype(end(c))
    { return end(c); }
template <typename T>
inline constexpr auto rbegin (T& c) -> decltype(c.rbegin())
    { return c.rbegin(); }
template <typename T>
inline constexpr auto rbegin (const T& c) -> decltype(c.rbegin())
    { return c.rbegin(); }
template <typename T>
inline constexpr auto crbegin (const T& c) -> decltype(rbegin(c))
    { return rbegin(c); }
template <typename T>
inline constexpr auto rend (T& c) -> decltype(c.rend())
    { return c.rend(); }
template <typename T>
inline constexpr auto rend (const T& c) -> decltype(c.rend())
    { return c.rend(); }
template <typename T>
inline constexpr auto crend (const T& c) -> decltype(rend(c))
    { return rend(c); }
template <typename T>
inline constexpr auto size (const T& c) -> decltype(c.size())
    { return c.size(); }
template <typename T, size_t N>
inline constexpr size_t size (const T (&)[N]) noexcept
    { return N; }
template <typename T>
inline constexpr bool empty (const T& c)
    { return !c.size(); }
template <typename T>
inline constexpr auto data (T& c) -> decltype(c.data())
    { return c.data(); }
template <typename T>
inline constexpr auto data (const T& c) -> decltype(c.data())
    { return c.data(); }
template <typename T, size_t N>
inline constexpr T* data(T (&c)[N]) noexcept
    { return &c[0]; }

template <typename T, size_t N>
inline constexpr T* VectorEnd (T (&a)[N]) noexcept
    { return end(a); }
#define VectorBlock(v)	::ustl::data(v), ::ustl::size(v)
#define VectorRange(v)	::ustl::begin(v), ::ustl::end(v)
#define foreach(type,i,ctr)	for (type i = ::ustl::begin(ctr); i != ::ustl::end(ctr); ++i)
#define eachfor(type,i,ctr)	for (type i = ::ustl::rbegin(ctr); i != ::ustl::rend(ctr); ++i)

/// Returns the size of \p n elements of size \p T
template <typename T> inline constexpr size_t size_of_elements (size_t n, const T*) { return n * sizeof(T); }
/// Returns the number of elements in a static vector
template <typename T>
constexpr auto VectorSize (const T&)
    { return 0; }
template <typename T, size_t N>
constexpr auto VectorSize (const T (&)[N])
    { return N; }

//}}}-------------------------------------------------------------------
//{{{ iterator

template <typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator {
    using value_type		= T;
    using difference_type	= Distance;
    using pointer		= Pointer;
    using reference		= Reference;
    using iterator_category	= Category;
};

//}}}-------------------------------------------------------------------
//{{{ iterator_traits

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag {};
struct bidirectional_iterator_tag {};
struct random_access_iterator_tag {};

/// \struct iterator_traits uiterator.h ustl.h
/// \brief Contains the type traits of \p Iterator
///
template <typename Iterator>
struct iterator_traits {
    using value_type		= typename Iterator::value_type;
    using difference_type	= typename Iterator::difference_type;
    using pointer		= typename Iterator::pointer;
    using reference		= typename Iterator::reference;
    using iterator_category	= typename Iterator::iterator_category;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <typename T>
struct iterator_traits<T*> {
    using value_type		= T;
    using difference_type	= ptrdiff_t;
    using const_pointer		= const T*;
    using pointer		= T*;
    using const_reference	= const T&;
    using reference		= T&;
    using iterator_category	= random_access_iterator_tag;
};

template <typename T>
struct iterator_traits<const T*> {
    using value_type		= T;
    using difference_type	= ptrdiff_t;
    using const_pointer		= const T*;
    using pointer		= const T*;
    using const_reference	= const T&;
    using reference		= const T&;
    using iterator_category	= random_access_iterator_tag;
};

template <>
struct iterator_traits<void*> {
    using value_type		= uint8_t;
    using difference_type	= ptrdiff_t;
    using const_pointer		= const void*;
    using pointer		= void*;
    using const_reference	= const value_type&;
    using reference		= value_type&;
    using iterator_category	= random_access_iterator_tag;
};

template <>
struct iterator_traits<const void*> {
    using value_type		= uint8_t;
    using difference_type	= ptrdiff_t;
    using const_pointer		= const void*;
    using pointer		= const void*;
    using const_reference	= const value_type&;
    using reference		= const value_type&;
    using iterator_category	= random_access_iterator_tag;
};

#endif

//}}}-------------------------------------------------------------------
//{{{ reverse_iterator

/// \class reverse_iterator uiterator.h ustl.h
/// \ingroup IteratorAdaptors
/// \brief Wraps \p Iterator to behave in an exactly opposite manner.
///
template <typename Iterator>
class reverse_iterator {
public:
    using value_type		= typename iterator_traits<Iterator>::value_type;
    using difference_type	= typename iterator_traits<Iterator>::difference_type;
    using pointer		= typename iterator_traits<Iterator>::pointer;
    using reference		= typename iterator_traits<Iterator>::reference;
    using iterator_category	= typename iterator_traits<Iterator>::iterator_category;
public:
    constexpr			reverse_iterator (void) : _i() {}
    constexpr explicit		reverse_iterator (Iterator iter) : _i (iter) {}
    constexpr bool		operator== (const reverse_iterator& iter) const { return _i == iter._i; }
    constexpr bool		operator< (const reverse_iterator& iter) const { return iter._i < _i; }
    constexpr Iterator		base (void) const { return _i; }
    constexpr reference		operator* (void) const { Iterator prev (_i); --prev; return *prev; }
    constexpr pointer		operator-> (void) const { return &(operator*()); }
    constexpr reverse_iterator&	operator++ (void) { -- _i; return *this; }
    constexpr reverse_iterator&	operator-- (void) { ++ _i; return *this; }
    constexpr reverse_iterator	operator++ (int) { reverse_iterator prev (*this); -- _i; return prev; }
    constexpr reverse_iterator	operator-- (int) { reverse_iterator prev (*this); ++ _i; return prev; }
    constexpr reverse_iterator&	operator+= (size_t n) { _i -= n; return *this; }
    constexpr reverse_iterator&	operator-= (size_t n) { _i += n; return *this; }
    constexpr reverse_iterator	operator+ (size_t n) const { return reverse_iterator (_i - n); }
    constexpr reverse_iterator	operator- (size_t n) const { return reverse_iterator (_i + n); }
    constexpr reference		operator[] (uoff_t n) const { return *(*this + n); }
    constexpr difference_type	operator- (const reverse_iterator& i) const { return distance (i._i, _i); }
protected:
    Iterator			_i;
};

template <typename Iterator>
constexpr reverse_iterator<Iterator> make_reverse_iterator (Iterator i)
    { return reverse_iterator<Iterator>(i); }

//}}}-------------------------------------------------------------------
//{{{ move_iterator

/// \class move_iterator uiterator.h ustl.h
/// \ingroup IteratorAdaptors
/// \brief Wraps \p Iterator to behave in an exactly opposite manner.
///
template <typename Iterator>
class move_iterator {
public:
    using value_type		= typename iterator_traits<Iterator>::value_type;
    using difference_type	= typename iterator_traits<Iterator>::difference_type;
    using pointer		= typename iterator_traits<Iterator>::pointer;
    using reference		= value_type&&;
    using iterator_category	= typename iterator_traits<Iterator>::iterator_category;
public:
    constexpr			move_iterator (void)		: _i() {}
    constexpr explicit		move_iterator (Iterator iter)	: _i (iter) {}
    constexpr bool		operator== (const move_iterator& iter) const	{ return _i == iter._i; }
    constexpr bool		operator< (const move_iterator& iter) const	{ return _i < iter._i; }
    constexpr Iterator		base (void) const		{ return _i; }
    constexpr reference		operator* (void) const		{ return move(*_i); }
    constexpr pointer		operator-> (void) const		{ return &*_i; }
    constexpr move_iterator&	operator++ (void)		{ ++_i; return *this; }
    constexpr move_iterator&	operator-- (void)		{ --_i; return *this; }
    constexpr move_iterator	operator++ (int)		{ move_iterator r (*this); ++ _i; return r; }
    constexpr move_iterator	operator-- (int)		{ move_iterator r (*this); -- _i; return r; }
    constexpr move_iterator&	operator+= (size_t n)		{ _i += n; return *this; }
    constexpr move_iterator&	operator-= (size_t n)		{ _i -= n; return *this; }
    constexpr move_iterator	operator+ (size_t n) const	{ return move_iterator (_i - n); }
    constexpr move_iterator	operator- (size_t n) const	{ return move_iterator (_i + n); }
    constexpr reference		operator[] (uoff_t n) const	{ return move(*(*this + n)); }
    constexpr difference_type	operator- (const move_iterator& i) const	{ return distance (_i, i._i); }
protected:
    Iterator			_i;
};

template <typename Iterator>
constexpr move_iterator<Iterator> make_move_iterator (Iterator i)
    { return move_iterator<Iterator>(i); }

//}}}-------------------------------------------------------------------
//{{{ insert_iterator

/// \class insert_iterator uiterator.h ustl.h
/// \ingroup IteratorAdaptors
/// \brief Calls insert on bound container for each assignment.
///
template <typename Container>
class insert_iterator {
public:
    using value_type		= typename Container::value_type;
    using difference_type	= typename Container::difference_type;
    using pointer		= typename Container::pointer;
    using reference		= typename Container::reference;
    using iterator		= typename Container::iterator;
    using iterator_category	= output_iterator_tag;
public:
    constexpr explicit		insert_iterator (Container& ctr, iterator ip) : _rctr (ctr), _ip (ip) {}
    inline insert_iterator&	operator= (typename Container::const_reference v)
				    { _ip = _rctr.insert (_ip, v); return *this; }
    constexpr insert_iterator&	operator* (void)  { return *this; }
    constexpr insert_iterator&	operator++ (void) { ++ _ip; return *this; }
    constexpr insert_iterator	operator++ (int)  { insert_iterator prev (*this); ++_ip; return prev; }
protected:
    Container&			_rctr;
    iterator			_ip;
};

/// Returns the insert_iterator for \p ctr.
template <typename Container>
constexpr insert_iterator<Container> inserter (Container& ctr, typename Container::iterator ip)
    { return insert_iterator<Container> (ctr, ip); }

//}}}-------------------------------------------------------------------
//{{{ back_insert_iterator

/// \class back_insert_iterator uiterator.h ustl.h
/// \ingroup IteratorAdaptors
/// \brief Calls push_back on bound container for each assignment.
///
template <class Container>
class back_insert_iterator {
public:
    using value_type		= typename Container::value_type;
    using difference_type	= typename Container::difference_type;
    using pointer		= typename Container::pointer;
    using reference		= typename Container::reference;
    using iterator_category	= output_iterator_tag;
public:
    constexpr explicit			back_insert_iterator (Container& ctr) : _rctr (ctr) {}
    inline back_insert_iterator&	operator= (typename Container::const_reference v)
					    { _rctr.push_back (v); return *this; }
    constexpr back_insert_iterator&	operator* (void)  { return *this; }
    constexpr back_insert_iterator&	operator++ (void) { return *this; }
    constexpr back_insert_iterator	operator++ (int)  { return *this; }
protected:
    Container&		_rctr;
};

/// Returns the back_insert_iterator for \p ctr.
template <class Container>
constexpr back_insert_iterator<Container> back_inserter (Container& ctr)
    { return back_insert_iterator<Container> (ctr); }

//}}}-------------------------------------------------------------------
//{{{ front_insert_iterator

/// \class front_insert_iterator uiterator.h ustl.h
/// \ingroup IteratorAdaptors
/// \brief Calls push_front on bound container for each assignment.
///
template <class Container>
class front_insert_iterator {
public:
    using value_type		= typename Container::value_type;
    using difference_type	= typename Container::difference_type;
    using pointer		= typename Container::pointer;
    using reference		= typename Container::reference;
    using iterator_category	= output_iterator_tag;
public:
    constexpr explicit			front_insert_iterator (Container& ctr) : _rctr (ctr) {}
    inline front_insert_iterator&	operator= (typename Container::const_reference v)
					    { _rctr.push_front (v); return *this; }
    constexpr front_insert_iterator&	operator* (void)  { return *this; }
    constexpr front_insert_iterator&	operator++ (void) { return *this; }
    constexpr front_insert_iterator	operator++ (int)  { return *this; }
protected:
    Container&		_rctr;
};

/// Returns the front_insert_iterator for \p ctr.
template <class Container>
constexpr front_insert_iterator<Container> front_inserter (Container& ctr)
    { return front_insert_iterator<Container> (ctr); }

//}}}-------------------------------------------------------------------
//{{{ index_iterate

/// \class index_iterate uiterator.h ustl.h
/// \ingroup IteratorAdaptors
///
/// \brief Allows iteration through an index container.
///
/// Converts an iterator into a container of uoff_t indexes to an
/// iterator of iterators into another container.
///
template <typename RandomAccessIterator, typename IndexIterator>
class index_iterate {
public:
    using value_type		= RandomAccessIterator;
    using difference_type	= ptrdiff_t;
    using pointer		= RandomAccessIterator*;
    using reference		= RandomAccessIterator;
    using iterator_category	= random_access_iterator_tag;
public:
    constexpr			index_iterate (void) : _base(), _i() {}
    constexpr			index_iterate (RandomAccessIterator ibase, IndexIterator iindex) : _base (ibase), _i (iindex) {}
    constexpr bool		operator== (const index_iterate& i) const { return _i == i._i; }
    constexpr bool		operator< (const index_iterate& i) const { return _i < i._i; }
    constexpr bool		operator== (const RandomAccessIterator& i) const { return _base == i; }
    constexpr bool		operator< (const RandomAccessIterator& i) const { return _base < i; }
    constexpr IndexIterator	base (void) const { return _i; }
    constexpr reference		operator* (void) const { return _base+*_i; }
    constexpr pointer		operator-> (void) const { return &(operator*()); }
    constexpr index_iterate&	operator++ (void) { ++_i; return *this; }
    constexpr index_iterate&	operator-- (void) { --_i; return *this; }
    constexpr index_iterate	operator++ (int) { index_iterate prev (*this); ++_i; return prev; }
    constexpr index_iterate	operator-- (int) { index_iterate prev (*this); --_i; return prev; }
    constexpr index_iterate&	operator+= (size_t n) { _i += n; return *this; }
    constexpr index_iterate&	operator-= (size_t n) { _i -= n; return *this; }
    constexpr index_iterate	operator+ (size_t n) const { return index_iterate (_base, _i + n); }
    constexpr index_iterate	operator- (size_t n) const { return index_iterate (_base, _i - n); }
    constexpr reference		operator[] (uoff_t n) const { return *(*this + n); }
    constexpr difference_type	operator- (const index_iterate& i) const { return distance (_i, i._i); }
private:
    RandomAccessIterator	_base;
    IndexIterator		_i;
};

/// Returns an index_iterate for \p ibase over \p iindex.
template <typename RandomAccessIterator, typename IndexIterator>
constexpr index_iterate<RandomAccessIterator, IndexIterator> index_iterator (RandomAccessIterator ibase, IndexIterator iindex)
{
    return index_iterate<RandomAccessIterator, IndexIterator> (ibase, iindex);
}

/// Converts the indexes in \p xc to iterators in \p ic of base \p ibase.
template <typename IndexContainer, typename IteratorContainer>
inline void indexv_to_iteratorv (typename IteratorContainer::value_type ibase, const IndexContainer& xc, IteratorContainer& ic)
{
    ic.resize (xc.size());
    copy_n (index_iterator (ibase, xc.begin()), xc.size(), ic.begin());
}

//}}}-------------------------------------------------------------------
//{{{ IBYI

/// Converts the given const_iterator into an iterator.
///
template <typename Container>
constexpr typename Container::iterator unconst (typename Container::const_iterator i, Container&)
    { return const_cast<typename Container::iterator>(i); }

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define IBYI(Iter1, Iter2, Ctr1, Ctr2)	\
template <typename Container1, typename Container2>	\
constexpr typename Container2::Iter2 ibyi (typename Container1::Iter1 idx, Ctr1& ctr1, Ctr2& ctr2)	\
{						\
    assert (ctr1.size() == ctr2.size());	\
    return ctr2.begin() + (idx - ctr1.begin());	\
}

IBYI(const_iterator, const_iterator, const Container1, const Container2)
IBYI(iterator, iterator, Container1, Container2)
IBYI(const_iterator, iterator, const Container1, Container2)
IBYI(iterator, const_iterator, Container1, const Container2)

#else // DOXYGEN

#error "This declaration is for doxygen only; it is not compiled."

/// Converts a const_iterator in one container into a const_iterator in another container.
template <typename Container1, typename Container2>
constexpr typename Container2::iterator ibyi (typename Container1::iterator idx, Container1& ctr1, Container2& ctr2) {}

#endif // DOXYGEN

//}}}-------------------------------------------------------------------

} // namespace ustl
