#pragma once

#include "value.hpp"

template<typename... Predicates>
struct types_satisfy_predicates;

template<typename Predicate>
struct types_satisfy_predicates<Predicate> {
	template<typename... Types>
	static constexpr bool for_types_of = Predicate::template for_types_of<Types...>;
};

template<typename HeadPredicate, typename... TailPredicates>
struct types_satisfy_predicates<HeadPredicate, TailPredicates...> {

	template<typename... Types>
	static constexpr bool for_types_of =
		HeadPredicate::template for_types_of<Types...>
		&& types_satisfy_predicates<TailPredicates...>::template for_types_of<Types...>
	;
};
