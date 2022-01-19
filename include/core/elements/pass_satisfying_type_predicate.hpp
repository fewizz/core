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
		struct function {
			F&& f;

			template<typename... Types>
			decltype(auto) for_elements_of(const Types&... elements) const {
				using indices = typename types::indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;

				using to_f = typename elements::pass_at<indices>::template function<F>;
				
				return to_f{ forward<F>(f) }.template for_elements_of<Types...>(elements...);
			}

			template<typename... Types>
			decltype(auto) operator () (const Types&... elements) const {
				return for_elements_of<Types...>(elements...);
			}
		};

		template<typename F>
		function(F&&) -> function<F>;
	};
}