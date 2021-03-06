// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once

namespace ustl {

/// \class queue uqueue.h ustl.h
/// \ingroup Sequences
///
/// Queue adapter to uSTL containers.
///
template <typename T, typename Container = vector<T> >
class queue {
public:
    using value_type		= T;
    using container_type	= Container;
    using size_type		= typename container_type::size_type;
    using difference_type	= typename container_type::difference_type;
    using reference		= value_type&;
    using const_reference	= const value_type&;
public:
    inline			queue (void)			: _storage(), _front (0) { }
    explicit inline		queue (const container_type& s)	: _storage (s), _front (0) { }
    explicit inline		queue (const queue& s)		: _storage (s._storage), _front (0) { }
    inline			queue (queue&& v)		: _storage(move(v._storage)),_front(v._front) { v._front = 0; }
    inline			queue (container_type&& s)	: _storage(move(s)),_front(0) {}
    inline queue&		operator= (queue&& v)		{ swap (v); return *this; }
    inline size_type		size (void) const		{ return _storage.size() - _front; }
    inline bool			empty (void) const		{ return !size(); }
    inline reference		front (void)			{ return _storage [_front]; }
    inline const_reference	front (void) const		{ return _storage [_front]; }
    inline reference		back (void)			{ return _storage.back(); }
    inline const_reference	back (void) const		{ return _storage.back(); }
    inline void			push (const_reference v)	{ _storage.push_back (v); }
    void			pop (void) {
				    if (++_front > _storage.size()/2) {
					_storage.erase (_storage.begin(), _front);
					_front = 0;
				    }
				}
    template <typename... Args>
    inline void			emplace (Args&&... args)	{ _storage.emplace_back (forward<Args>(args)...); }
    inline void			swap (queue& v)			{ _storage.swap (v); swap (_front, v._front); }
    inline bool			operator== (const queue& s) const	{ return _storage == s._storage && _front == s._front; }
    inline bool			operator< (const queue& s) const	{ return size() < s.size(); }
private:
    container_type		_storage;	///< Where the data actually is.
    size_type			_front;	///< Index of the element returned by next pop.
};

} // namespace ustl
