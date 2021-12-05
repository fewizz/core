#pragma once

#include "indices_of_type.hpp"
#include "count_of_type.hpp"
#include "../values/at_index.hpp"

namespace types {

	template<typename Type>
	struct index_of_type {

		template<typename... Types>
		requires(types::count_of_type<Type>::template for_types_of<Types...> == 1)
		static constexpr nuint for_types_of =
			types::indices_of_type<Type>::template for_types_of<Types...>::template pass_for_value<values::at_index<0>>;
	};
}