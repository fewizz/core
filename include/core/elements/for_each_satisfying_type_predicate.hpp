#pragma once

#include "../type/decay.hpp"
#include "../type/predicate.hpp"
#include "../type/modified_predicate.hpp"
#include "../types/indices_of_satisfying_predicate.hpp"
#include "of.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct for_each_satisfying_type_predicate_t {

		template<typename... Types>
		struct acceptor {
			elements::of<Types...> elements;

			template<typename F>
			void operator () (F&& f) {
				elements.for_each(
					forward<F>(f),
					typename types::indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>{}
				);
			}
		};

		template<typename... Types>
		auto operator () (Types&&... elements) const {
			return acceptor<Types...> {
				{ forward<Types>(elements)... }
			};
		}

	};

	template<type::predicate Predicate>
	inline constexpr auto for_each_satisfying_type_predicate = elements::for_each_satisfying_type_predicate_t<Predicate>{};
}