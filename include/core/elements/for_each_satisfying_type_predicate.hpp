#pragma once

#include "../type/predicate.hpp"
#include "../type/modified_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct for_each_satisfying_type_predicate {

		template<typename F>
		struct function {
			F&& f;

			template<typename Head, typename... Types>
			void for_elements_of(Head&& head_element, Types&&... elements) const {
				if constexpr(Predicate::template for_type<Head>) {
					f(head_element);
				}
				if constexpr(sizeof...(Types) > 0) {
					for_elements_of<Types...>(forward<Types>(elements)...);
				}
			}
		};

		template<typename F>
		function(F&& f) -> function<F>;

		using ignore_reference = elements::for_each_satisfying_type_predicate<type::modified_predicate<Predicate, type::remove_reference>>;
		using ignore_const = elements::for_each_satisfying_type_predicate<type::modified_predicate<Predicate, type::remove_const>>;
	};
}