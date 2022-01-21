#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "../values/first.hpp"
#include "count_of_satisfying_predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	struct index_of_satisfying_predicate {

		template<typename... Types>
		requires(types::count_of_satisfying_predicate<Predicate>::template for_types_of<Types...> == 1)
		static constexpr nuint for_types_of =
			types::indices_of_satisfying_predicate<Predicate>
			::template for_types_of<Types...>
			::template pass_for_value<values::first>;

	};
}