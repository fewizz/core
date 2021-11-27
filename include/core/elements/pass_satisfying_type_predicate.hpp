#pragma once

#include "../type/predicate.hpp"
#include "../types/indices_of_satisfying_predicate.hpp"
#include "pass_at_indices.hpp"
#include "of.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct pass_satisfying_type_predicate {

		template<typename F, typename... Types>
		static decltype(auto) for_function_and_elements_of(F&& f, Types&&... elements) {
			using indices = typename types::indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;
			return elements::pass_at<indices>::for_function_and_elements_of(forward<F>(f), forward<Types>(elements)...);
		}
	};
}