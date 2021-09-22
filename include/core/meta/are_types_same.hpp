#pragma once

#include "value.hpp"

template<typename... Types>
requires(sizeof...(Types) >= 2)
constexpr inline bool are_types_same = false;

template<typename T>
constexpr inline bool are_types_same<T, T> = true;

template<typename Head, typename... Tail>
constexpr inline bool are_types_same<Head, Head, Tail...>
	= are_types_same<Head, Tail...>
;

template<typename... Types>
constexpr inline bool are_same = are_types_same<Types...>;

template<typename... Types>
using are_types_same_predicate = value::of<are_types_same<Types...>>;