// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "umap.h"

namespace ustl {

/// \class multimap umultimap.h ustl.h
/// \ingroup AssociativeContainers
///
/// \brief A sorted associative container that may container multiple entries for each key.
///
template <typename K, typename V, typename Comp = less<K> >
class multimap : public vector<pair<K,V> > {
public:
    using key_type		= K;
    using data_type		= V;
    using const_key_ref		= const K&;
    using const_data_ref	= const V&;
    using rcself_t		= const multimap<K,V,Comp>&;
    using base_class		= vector<pair<K,V> >;
    using value_type		= typename base_class::value_type;
    using size_type		= typename base_class::size_type;
    using pointer		= typename base_class::pointer;
    using const_pointer		= typename base_class::const_pointer;
    using reference		= typename base_class::reference;
    using const_reference	= typename base_class::const_reference;
    using const_iterator	= typename base_class::const_iterator;
    using iterator		= typename base_class::iterator;
    using reverse_iterator	= typename base_class::reverse_iterator;
    using const_reverse_iterator = typename base_class::const_reverse_iterator;
    using const_range_t		= pair<const_iterator,const_iterator>;
    using range_t		= pair<iterator,iterator>;
    using key_compare		= Comp;
    using value_compare		= pair_compare_first<value_type,Comp>;
    using value_key_compare	= pair_compare_first_key<K,V,Comp>;
    using initlist_t		= std::initializer_list<value_type>;
public:
    inline			multimap (void)		: base_class() {}
    explicit inline		multimap (size_type n)	: base_class (n) {}
    inline			multimap (rcself_t v)	: base_class (v) {}
    inline			multimap (const_iterator i1, const_iterator i2)	: base_class() { insert (i1, i2); }
    inline			multimap (multimap&& v)	: base_class (move(v)) {}
    inline			multimap (initlist_t v)	: base_class() { insert (v.begin(), v.end()); }
    inline rcself_t		operator= (rcself_t v)	{ base_class::operator= (v); return *this; }
    inline multimap&		operator= (multimap&& v){ base_class::operator= (move(v)); return *this; }
    inline key_compare		key_comp (void) const	{ return key_compare(); }
    inline value_compare	value_comp (void) const	{ return value_compare(); }
    inline size_type		size (void) const	{ return base_class::size(); }
    inline iterator		begin (void)		{ return base_class::begin(); }
    inline const_iterator	begin (void) const	{ return base_class::begin(); }
    inline iterator		end (void)		{ return base_class::end(); }
    inline const_iterator	end (void) const	{ return base_class::end(); }
    inline const_iterator	find (const_key_ref k) const;
    inline iterator		find (const_key_ref k)			{ return const_cast<iterator> (const_cast<rcself_t>(*this).find (k)); }
    const_iterator		lower_bound (const_key_ref k) const	{ return ::ustl::lower_bound (begin(), end(), k, value_key_compare()); }
    inline iterator		lower_bound (const_key_ref k)		{ return const_cast<iterator>(const_cast<rcself_t>(*this).lower_bound (k)); }
    const_iterator		upper_bound (const_key_ref k) const	{ return ::ustl::upper_bound (begin(), end(), k, value_key_compare()); }
    inline iterator		upper_bound (const_key_ref k)		{ return const_cast<iterator>(const_cast<rcself_t>(*this).upper_bound (k)); }
    const_range_t		equal_range (const_key_ref k) const	{ return ::ustl::equal_range (begin(), end(), k, value_key_compare()); }
    inline range_t		equal_range (const_key_ref k)		{ return ::ustl::equal_range (begin(), end(), k, value_key_compare()); }
    inline size_type		count (const_key_ref v) const		{ const_range_t r = equal_range(v); return distance(r.first,r.second); }
    inline void			assign (const_iterator i1, const_iterator i2) { clear(); insert (i1, i2); }
    inline void			push_back (const_reference v)		{ insert (v); }
    template <typename... Args>
    inline iterator		emplace (Args&&... args)		{ return insert (value_type(forward<Args>(args)...)); }
    template <typename... Args>
    inline iterator		emplace_hint (const_iterator h, Args&&... args)	{ return insert (h, value_type(forward<Args>(args)...)); }
    template <typename... Args>
    inline iterator		emplace_back (Args&&... args)		{ return insert (value_type(forward<Args>(args)...)); }
    inline iterator		insert (const_reference v)		{ return base_class::insert (upper_bound (v.first), v); }
    void			insert (const_iterator i1, const_iterator i2)	{ for (; i1 != i2; ++i1) insert (*i1); }
    iterator			insert (value_type&& v)			{ return base_class::insert (upper_bound (v.first), move(v)); }
    inline iterator		insert (const_iterator, value_type&& v)	{ return insert(move(v)); }
    inline void			insert (initlist_t v)			{ insert (v.begin(), v.end()); }
    inline void			clear (void)				{ base_class::clear(); }
    inline void			erase (const_key_ref k)			{ erase (const_cast<iterator>(lower_bound(k)), const_cast<iterator>(upper_bound(k))); }
    inline iterator		erase (const_iterator ep)			{ return base_class::erase (ep); } 
    inline iterator		erase (const_iterator ep1, const_iterator ep2)	{ return base_class::erase (ep1, ep2); } 
    inline void			swap (multimap& v)			{ base_class::swap (v); }
};

/// Returns the pair<K,V> where K = \p k.
template <typename K, typename V, typename Comp>
inline typename multimap<K,V,Comp>::const_iterator multimap<K,V,Comp>::find (const_key_ref k) const
{
    const_iterator i = lower_bound (k);
    return (i < end() && Comp()(k, i->first)) ? end() : i;
}

} // namespace ustl
