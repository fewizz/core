#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "count_of_satisfying_predicate.hpp"
#include "../values/first.hpp"

namespace types {

	template<type::predicate Predicate>
	struct index_of_satisfying_predicate {

		template<typename... Types>
		requires(
			types::count_of_satisfying_predicate<Predicate>::template
			for_types<Types...> == 1
		)
		static constexpr nuint for_types =
			types::indices_of_satisfying_predicate<Predicate>
			::template for_types<Types...>
			::template pass_for_value<values::first>;

	};

} // types