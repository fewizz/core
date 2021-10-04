#pragma once

#include "../array.hpp"
#include "../types/indices_of_type.hpp"

namespace elements {

	template<typename Type>
	struct array_of_copy_of_elements_of_type {

		template<typename... Elements>
		auto for_elements_of(Elements&&... elements) {
			using indices = typename types::indices_of_type<Type>::template for_types_of<Elements...>;
			return array<Type, indices::size> {
				
			};
		}
	};

}