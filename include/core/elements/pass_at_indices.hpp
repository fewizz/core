#pragma once

#include "of.hpp"
#include "at_index.hpp"

namespace elements {

	template<nuint... Indices>
	struct pass_at_indices {

		template<typename... Types>
		static decltype(auto) for_function_and_elements_of(auto&& f, Types&&... elements) {
			return f(elements::at_index<Indices>::for_elements_of(elements...)...);
		}
	};

	template<typename Indices>
	struct pass_at;

	template<nuint... Indices>
	struct pass_at<indices::of<Indices...>> : pass_at_indices<Indices...> {};
}