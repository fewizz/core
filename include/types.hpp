#pragma once

#include "./__types/predicate.hpp"

#include "./__types/exclusively_satisfy_predicates.hpp"
#include "./__types/are_same.hpp"
#include "./__types/satisfy_predicates.hpp"
#include "./__types/at_index.hpp"
#include "./__types/at_indices.hpp"
#include "./__types/common.hpp"
#include "./__types/count_of_satisfying_predicate.hpp"
#include "./__types/erase_at_index.hpp"
#include "./__types/first.hpp"
#include "./__types/index_of_satisfying_predicate.hpp"
#include "./__types/indices_of_satisfying_predicate.hpp"

template<typename... Types>
struct common_if_have {
	static constexpr bool have_common = false;
};

template<typename... Types>
requires requires { typename __types::common::for_types<Types...>; }
struct common_if_have<Types...> {
	static constexpr bool have_common = true;
	using common = __types::common::for_types<Types...>;
};

template<typename... Types>
struct types : common_if_have<Types...> {

	template</*types_predicate*/ auto... Predicates>
	static constexpr bool exclusively_satisfy_predicates =
		__types::exclusively_satisfy_predicates<Predicates...>::template
		for_types<Types...>;

	static constexpr bool are_same = types_are_same<Types...>;

	template</*types_predicate*/ auto... Predicates>
	static constexpr bool satisfy_predicates =
		__types::satisfy_predicates<Predicates...>::template
		for_types<Types...>;

	template<nuint Index>
	using at_index = type_at_index<Index, Types...>;

	template<nuint... Indices>
	using at_indices = types<__type_pack_element<Indices, Types...>...>;

	template<type_predicate auto Predicate>
	static constexpr nuint count_of_satisfying_predicate
		= ::count_of_satisfying_predicate<Predicate>.template
		for_types<Types...>;

	template<nuint Index>
	using erase_at_index = typename
		__types::erase_at_index<Index>::template
		for_types<Types...>::template
		pass_for_type_directly<types>;

	using first = __types::first::for_types<Types...>;

	template<type_predicate auto Predicate>
	static constexpr nuint index_of_satisfying_predicate =
		__types::index_of_satisfying_predicate<Predicate>::template
		for_types<Types...>;

	template<type_predicate auto Predicate>
	using indices_of_satisfying_predicate = typename
		__types::indices_of_satisfying_predicate<Predicate>::template
		for_types<Types...>;

};