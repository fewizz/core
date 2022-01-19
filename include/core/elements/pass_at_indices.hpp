#pragma once

#include "of.hpp"
#include "at_index.hpp"
#include "../types/at_index.hpp"
#include "../forward.hpp"
#include "of.hpp"

namespace elements {

	template<nuint... Indices>
	struct pass_at_indices {

		template<typename F>
		struct function {
			F&& f;

			template<typename... Types>
			decltype(auto) for_elements_of(const Types&... elements) const {
				return f(elements::at_index<Indices>.for_elements_of(elements...)...);
			}

			template<typename... Types>
			decltype(auto) for_elements(const elements::of<Types...>& elements) const {
				return f(elements.at_index<Indices>()...);
			}
		};

		template<typename F>
		function(F&&) -> function<F>;
	};

	template<typename Indices>
	struct pass_at;

	template<nuint... Indices>
	struct pass_at<indices::of<Indices...>> : pass_at_indices<Indices...> {};
}