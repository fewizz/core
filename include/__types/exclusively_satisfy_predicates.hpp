#pragma once

#include "./predicate.hpp"

namespace __types {

	template<types_predicate auto... Predicates>
	class exclusively_satisfy_predicates : public types_predicate_mark {

		template<typename Type>
		static constexpr bool is_affected_by_only_one_predicate =
			(
				Predicates.affecting_predicate().template for_type<Type>()
				+ ...
			) == 1;

	public:

		template<typename... Types>
		static constexpr bool for_types =
			(Predicates.template for_types<Types...>() && ...) &&
			(is_affected_by_only_one_predicate<Types> && ...);

	};

}