#pragma once

#include "predicate.hpp"
#include "erase_at_indices.hpp"

#include "../value/of.hpp"

namespace types {

	template<types::predicate... Predicates>
	class are_exclusively_satisfying_predicates :
		public types::predicate_marker
	{

		template<typename Type>
		static constexpr bool is_affected_by_one_predicate =
			(nuint{
				Predicates::affecting_predicate::template for_type<Type>
			} + ...) == 1;

	public:

		template<typename... Types>
		static constexpr bool for_types =
			(Predicates::template for_types<Types...> && ...) &&
			(is_affected_by_one_predicate<Types> && ...);

	};

} // types