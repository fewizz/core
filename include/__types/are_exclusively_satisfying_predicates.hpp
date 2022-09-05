#pragma once

#include "./predicate.hpp"
#include "./erase_at_indices.hpp"
#include "../value/of.hpp"

namespace __types {

	template<types_predicate auto... Predicates>
	class are_exclusively_satisfying_predicates :
		public types_predicate_marker
	{

		template<typename Type>
		static constexpr bool is_affected_by_one_predicate =
			(
				nuint {
					Predicates.affecting_predicate::template for_type<Type>
				} + ...
			) == 1;

	public:

		template<typename... Types>
		constexpr bool for_types() const {
			return (Predicates.template for_types<Types...> && ...) &&
			(is_affected_by_one_predicate<Types> && ...);
		};

	};

}