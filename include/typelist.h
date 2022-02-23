// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2007 by Mike Sharov <msharov@users.sourceforge.net>
//
// This implementation is adapted from the Loki library, distributed under
// the MIT license with Copyright (c) 2001 by Andrei Alexandrescu.

#pragma once
#include "metamac.h"
#include "typet.h"

namespace ustl {
namespace tm {

/// The building block of typelists. Use it throught the Seq templates.
template <typename T, typename U>
struct Typelist {
    using Head = T;
    using Tail = U;
};

/// Namespace containing typelist-related functionality.
namespace tl {

//----------------------------------------------------------------------
// Seq template definitions. The macros expand to a spec per arg count
//
#define TL_MAX_SEQ_TYPES		9
#define TL_MAX_SEQ_SPECS		8
#define TL_SEQ_TYPE(n)			T##n
#define TL_SEQ_TYPENAME(n)		typename TL_SEQ_TYPE(n)
#define TL_SEQ_NULLTYPE_DEFAULT(n)	TL_SEQ_TYPENAME(n)=NullType
#define TL_SEQ_TL_END(n)		> 
#define TL_SEQ_ONE_TYPELIST(n)		Typelist<TL_SEQ_TYPE(n)

/// Creates a typelist from a sequence of types
template <COMMA_LIST(TL_MAX_SEQ_TYPES,TL_SEQ_NULLTYPE_DEFAULT)>
struct Seq {
    using Type = COMMA_LIST(TL_MAX_SEQ_TYPES,TL_SEQ_ONE_TYPELIST),
	NullType REPEAT(TL_MAX_SEQ_TYPES,TL_SEQ_TL_END);
};

#define TL_SEQ_SPEC(n)	\
template <COMMA_LIST (n, TL_SEQ_TYPENAME)>	\
struct Seq<COMMA_LIST (n, TL_SEQ_TYPE)> {	\
    using Type = COMMA_LIST(n,TL_SEQ_ONE_TYPELIST),\
	NullType REPEAT(n,TL_SEQ_TL_END);	\
}
LIST(TL_MAX_SEQ_SPECS,TL_SEQ_SPEC,;);

#undef TL_SEQ_SPEC
#undef TL_SEQ_TL_END
#undef TL_SEQ_ONE_TYPELIST
#undef TL_SEQ_NULLTYPE_DEFAULT
#undef TL_SEQ_TYPE
#undef TL_MAX_SEQ_SPECS

//----------------------------------------------------------------------
// Various utility functions follow.

/// Length<List>::value is the number of types in the typelist.
template <typename List> struct Length { };
template <> struct Length<NullType> { enum { value = 0 }; };
template <typename T, typename U>
struct Length<Typelist<T, U> > { enum { value = 1 + Length<U>::value }; };

/// TypeAt<List, i>::Result is the ith type in List
template <typename List, unsigned index> struct TypeAt { };
template <class Head, class Tail>
struct TypeAt<Typelist<Head, Tail>, 0> {
    using Result = Head;
};
template <class Head, class Tail, unsigned index>
struct TypeAt<Typelist<Head, Tail>, index> {
    using Result = typename TypeAt<Tail, index-1>::Result;
};

/// TypeAtNonStrict<List,i,DefaultType>::Result is List[i] or DefaultType if out of range.
template <typename List, unsigned index, typename DefaultType = NullType>
struct TypeAtNonStrict {
    using Result = DefaultType;
};
template <typename Head, typename Tail, typename DefaultType>
struct TypeAtNonStrict<Typelist<Head, Tail>, 0, DefaultType> {
    using Result = Head;
};
template <typename Head, typename Tail, unsigned index, typename DefaultType>
struct TypeAtNonStrict<Typelist<Head, Tail>, index, DefaultType> {
    using Result = typename TypeAtNonStrict<Tail, index-1, DefaultType>::Result;
};

/// IndexOf<List,T>::value is the position of T in List, or -1 if not found.
template <typename List, typename T> struct IndexOf;
template <typename T>
struct IndexOf<NullType, T> { enum { value = -1 }; };
template <typename T, typename Tail>
struct IndexOf<Typelist<T, Tail>, T> { enum { value = 0 }; };
template <typename Head, typename Tail, typename T>
struct IndexOf<Typelist<Head, Tail>, T> {
private:
    enum { iintail = IndexOf<Tail, T>::value };
public:
    enum { value = (iintail == -1 ? -1 : 1+iintail) };
};

/// Appends a type or a typelist to another in Append<TList, T>::Result
template <typename List, typename T> struct Append;
template <> struct Append<NullType, NullType> { using Result = NullType; };
template <typename T> struct Append<NullType, T> {
    using Result = Typelist<T,NullType>;
};
template <typename Head, typename Tail>
struct Append<NullType, Typelist<Head, Tail> > {
    using Result = Typelist<Head, Tail>;
};
template <typename Head, typename Tail, typename T>
struct Append<Typelist<Head, Tail>, T> {
    using Result = Typelist<Head, typename Append<Tail, T>::Result>;
};
        
// Erase<List, T>::Result contains List without the first T.
template <typename TList, typename T> struct Erase;
template <typename T>
struct Erase<NullType, T> { using Result = NullType; };
template <typename T, typename Tail>
struct Erase<Typelist<T, Tail>, T> { using Result = Tail; };
template <typename Head, typename Tail, typename T>
struct Erase<Typelist<Head, Tail>, T> {
    using Result = Typelist<Head, typename Erase<Tail, T>::Result>;
};

// EraseAll<List, T>::Result contains List without any T.
template <typename List, typename T> struct EraseAll;
template <typename T>
struct EraseAll<NullType, T> { using Result = NullType; };
template <typename T, typename Tail>
struct EraseAll<Typelist<T, Tail>, T> {
    using Result = typename EraseAll<Tail, T>::Result;
};
template <typename Head, typename Tail, typename T>
struct EraseAll<Typelist<Head, Tail>, T> {
    using Result = Typelist<Head, typename EraseAll<Tail, T>::Result>;
};

/// Removes all duplicate types in a typelist
template <typename List> struct NoDuplicates;
template <> struct NoDuplicates<NullType> { using Result = NullType; };
template <typename Head, typename Tail>
struct NoDuplicates< Typelist<Head, Tail> > {
private:
    using L1 = typename NoDuplicates<Tail>::Result;
    using L2 = typename Erase<L1, Head>::Result;
public:
    using Result = Typelist<Head, L2>;
};

// Replaces the first occurence of a type in a typelist, with another type
template <typename List, typename T, typename U> struct Replace;
template <typename T, typename U>
struct Replace<NullType, T, U> { using Result = NullType; };
template <typename T, typename Tail, typename U>
struct Replace<Typelist<T, Tail>, T, U> {
    using Result = Typelist<U, Tail>;
};
template <typename Head, typename Tail, typename T, typename U>
struct Replace<Typelist<Head, Tail>, T, U> {
    using Result = Typelist<Head, typename Replace<Tail, T, U>::Result>;
};

// Replaces all occurences of a type in a typelist, with another type
template <typename List, typename T, typename U> struct ReplaceAll;
template <typename T, typename U>
struct ReplaceAll<NullType, T, U> { using Result = NullType; };
template <typename T, typename Tail, typename U>
struct ReplaceAll<Typelist<T, Tail>, T, U> {
    using Result = Typelist<U, typename ReplaceAll<Tail, T, U>::Result>;
};
template <typename Head, typename Tail, typename T, typename U>
struct ReplaceAll<Typelist<Head, Tail>, T, U> {
    using Result = Typelist<Head, typename ReplaceAll<Tail, T, U>::Result>;
};

// Reverses a typelist
template <typename List> struct Reverse;
template <> struct Reverse<NullType> { using Result = NullType; };
template <typename Head, typename Tail>
struct Reverse< Typelist<Head, Tail> > {
    using Result = typename Append<typename Reverse<Tail>::Result, Head>::Result;
};

// Finds the type in a typelist that is the most derived from a given type
template <typename List, typename T> struct MostDerived;
template <typename T> struct MostDerived<NullType, T> { using Result = T; };
template <typename Head, typename Tail, typename T>
struct MostDerived<Typelist<Head, Tail>, T> {
private:
    using Candidate = typename MostDerived<Tail, T>::Result;
public:
    using Result = typename Select<SuperSubclass<Candidate,Head>::value, Head, Candidate>::Result;
};

// Arranges the types in a typelist so that the most derived types appear first
template <typename List> struct DerivedToFront;
template <> struct DerivedToFront<NullType> { using Result = NullType; };
template <typename Head, typename Tail>
struct DerivedToFront< Typelist<Head, Tail> > {
private:
    using TheMostDerived = typename MostDerived<Tail, Head>::Result;
    using Temp = typename Replace<Tail, TheMostDerived, Head>::Result;
    using L = typename DerivedToFront<Temp>::Result;
public:
    using Result = Typelist<TheMostDerived, L>;
};

//----------------------------------------------------------------------

} // namespace tl
} // namespace tm
} // namespace ustl
