#pragma once

#include "indices.hpp"
#include "satisfying.hpp"
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

template<typename Type0>
struct is_same {

	template<typename Type1>
	static constexpr bool as = are_types_same<Type0, Type1>;

	template<typename Type1>
	using as_predicate = value::of<is_same<Type0>::as<Type1>>;
};

template<typename Type>
struct indices_of_same_as {

	template<typename... Types>
	using of_types = typename indices_of_satisfying<
		is_same<Type>::template as_predicate
	>::template of_types<Types...>;
};

template<typename T0, typename T1>
concept same_as = are_same<T0, T1>;

template<typename T0, typename T1>
concept not_same_as = !are_same<T0, T1>;