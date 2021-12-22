#pragma once

#include "../type/predicate.hpp"
#include "index_of_satisfying_predicate.hpp"
#include "at_index.hpp"

namespace types {

	template<type::predicate Predicate>
	struct one_satisfying_predicate {
		
		template<typename... Types>
		using for_types_of =
			typename types::at_index<
				types::index_of_satisfying_predicate<Predicate>::template
				for_types_of<Types...>
			>::template for_types_of<Types...>;
	};
}