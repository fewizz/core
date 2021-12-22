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
		struct to_function {
			F&& f;

			template<typename... Types>
			decltype(auto) for_elements_of(Types&&... elements) const {
				return f(
					forward<
						typename types::at_index<Indices>::template for_types_of<Types...>
					>
					(
						elements::at_index<Indices>::for_elements_of(elements...)
					)...
				);
			}

			template<typename... Types>
			decltype(auto) for_elements(elements::of<Types...>&& elements) const {
				return f(
					forward<
						typename types::at_index<Indices>::template for_types_of<Types...>
					>
					(
						elements.at_index<Indices>()
					)...
				);
			}
		};

		template<typename F>
		to_function(F&&) -> to_function<F>;
	};

	template<typename Indices>
	struct pass_at;

	template<nuint... Indices>
	struct pass_at<indices::of<Indices...>> : pass_at_indices<Indices...> {};
}