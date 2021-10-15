#pragma once

#include "for_each_satisfying_type_predicate.hpp"
#include "../type/is_same_as.hpp"
namespace elements {

	template<typename Type, typename F, typename... Types>
	void for_each_of_type(F&& f, Types&&... elements) {
		return for_each_satisfying_type_predicate<type::is_same_as<Type>>(
			forward<F>(f),
			forward<Types>(elements)...
		);
	}

	template<typename Type, typename F, typename... Types>
	void for_each_of_type_remove_reference(F&& f, Types&&... elements) {
		return for_each_satisfying_type_predicate<typename type::is_same_as<Type>::remove_reference>(
			forward<F>(f),
			forward<Types>(elements)...
		);
	}
}