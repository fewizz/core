#pragma once

#include <type_traits>

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
using are_types_same_predicate = std::bool_constant<are_types_same<Types...>>;