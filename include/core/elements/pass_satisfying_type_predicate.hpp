#pragma once

#include "../type/predicate.hpp"
#include "../types/indices_of_satisfying_predicate.hpp"
#include "../types/at.hpp"
#include "pass_at_indices.hpp"
#include "of.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct pass_satisfying_type_predicate {

		template<typename F>
		struct to_function {
			F&& f;

			template<typename... Types>
			decltype(auto) for_elements_of(Types&&... elements) const {
				using is = typename types::indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;

				using to_f = typename elements::pass_at<is>::template to_function<F>;
				
				return to_f{ forward<F>(f) }.template for_elements_of<Types...>(forward<Types>(elements)...);
			}
		};

		template<typename F>
		to_function(F&&) -> to_function<F>;
	};
}