// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once

namespace ustl {

/// \class stack ustack.h ustl.h
/// \ingroup Sequences
///
/// Stack adapter to uSTL containers.
///
template <typename T, typename Container = vector<T> >
class stack {
public:
    using value_type		= T;
    using container_type	= Container;
    using size_type		= typename container_type::size_type;
    using difference_type	= typename container_type::difference_type;
    using reference		= value_type&;
    using const_reference	= const value_type&;
public:
    inline			stack (void)			: _storage () { }
    explicit inline		stack (const container_type& s)	: _storage (s) { }
    explicit inline		stack (const stack& s)		: _storage (s._storage) { }
    inline			stack (stack&& v)		: _storage(move(v._storage)) {}
    inline			stack (container_type&& s)	: _storage(move(s)) {}
    inline stack&		operator= (stack&& v)		{ swap (v); return *this; }
    inline bool			empty (void) const		{ return _storage.empty(); }
    inline size_type		size (void) const		{ return _storage.size(); }
    inline reference		top (void)			{ return _storage.back(); }
    inline const_reference	top (void) const		{ return _storage.back(); }
    inline void			push (const_reference v)	{ _storage.push_back (v); }
    template <typename... Args>
    inline void			emplace (Args&&... args)	{ _storage.emplace_back (forward<Args>(args)...); }
    inline void			pop (void)			{ _storage.pop_back(); }
    inline void			swap (stack& v)			{ _storage.swap (v); }
    inline bool			operator== (const stack& s) const	{ return _storage == s._storage; }
    inline bool			operator< (const stack& s) const	{ return _storage.size() < s._storage.size(); }
private:
    container_type		_storage;	///< Where the data actually is.
};

} // namespace ustl
