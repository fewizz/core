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
			elements::of<const Types&...> elements;

			auto operator () (auto&& f) {
				elements.for_each(
					f,
					typename types::indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>{}
				);
			}
		};

		template<typename Head, typename... Types>
		auto for_elements_of(const Head& head_element, const Types&... elements) const {
			return acceptor<Head, Types...> {
				.elements { head_element, elements... } 
			};
		}

		template<typename Head, typename... Types>
		auto operator () (const Head& head_element, const Types&... elements) const {
			return for_elements_of<Head, Types...>(head_element, elements...);
		}

	};

	template<type::predicate Predicate>
	inline constexpr auto for_each_satisfying_type_predicate = elements::for_each_satisfying_type_predicate_t<Predicate>{};
}