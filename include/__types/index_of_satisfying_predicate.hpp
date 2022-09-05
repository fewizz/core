#pragma once

#include "./indices_of_satisfying_predicate.hpp"
#include "./count_of_satisfying_predicate.hpp"
#include "../values/first.hpp"

namespace __types {

	template<type_predicate auto Predicate>
	struct index_of_satisfying_predicate {

		template<typename... Types>
		requires(
			count_of_satisfying_predicate<Predicate>.template
			for_types<Types...> == 1
		)
		static constexpr nuint for_types =
			__types::indices_of_satisfying_predicate<Predicate>
			::template for_types<Types...>
			::template pass_for_value<values::first>;

	};

} // types