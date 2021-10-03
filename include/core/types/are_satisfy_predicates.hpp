#pragma once

#include "../values/of.hpp"
#include "predicate.hpp"

namespace types {

	template<types::predicate... Predicates>
	struct are_satisfy_predicates;

	template<types::predicate Predicate>
	struct are_satisfy_predicates<Predicate> {
		template<typename... Types>
		static constexpr bool for_types_of = Predicate::template for_types_of<Types...>;
	};

	template<types::predicate HeadPredicate, types::predicate... TailPredicates>
	struct are_satisfy_predicates<HeadPredicate, TailPredicates...> {

		template<typename... Types>
		static constexpr bool for_types_of =
			HeadPredicate::template for_types_of<Types...>
			&& are_satisfy_predicates<TailPredicates...>::template for_types_of<Types...>
		;
	};

}