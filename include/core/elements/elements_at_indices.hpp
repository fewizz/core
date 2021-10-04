#pragma once

#include "../integer.hpp"
#include "at_index.hpp"
#include "of.hpp"

namespace elements {

	template<uint... Indices>
	struct at_indices {

		template<typename... Elements>
		auto for_elements_of(Elements&&... elements) {
			return elements::of {
				elements::element_at_index<Indices>::for_elements_of()
			};
		} 
	};

}