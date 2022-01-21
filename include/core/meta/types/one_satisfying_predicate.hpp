#pragma once

#include "at_index.hpp"
#include "index_of_satisfying_predicate.hpp"
#include "../type/predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	struct one_satisfying_predicate {
		
		template<typename... Types>
		using for_types =
			typename types::at_index<
				types::index_of_satisfying_predicate<Predicate>::template
				for_types<Types...>
			>::template for_types<Types...>;
	};
}