#pragma once

#include "../type/predicate.hpp"

namespace elements {

	template<type::predicate Predicate,  typename F, typename Head, typename... Types>
	void for_each_satisfying_type_predicate(F&& f, Head&& head_element, Types&&... elements) {
		if constexpr(Predicate::template for_type_of<Head>) {
			f(head_element);
		}

		if constexpr(sizeof...(Types) > 0) {
			for_each_satisfying_type_predicate<Predicate, F, Types...>(forward<F>(f), forward<Types>(elements)...);
		}
	}
}