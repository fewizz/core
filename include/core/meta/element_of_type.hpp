#pragma once

#include "element_at.hpp"
#include "indices_of_type.hpp"
#include "value_at.hpp"

template<typename Type>
struct element_of_type {

	template<typename... Types>
	static constexpr decltype(auto) for_elements_of(Types&&... elements) {
		using indices = typename indices_of_type<Type>::template for_types_of<Types...>;

		static_assert(indices::size == 1u);

		constexpr auto index = indices::template pass_for_value<value_at<0u>>;

		return element_at<index>::template for_elements_of<Types...>(forward<Types>(elements)...);
	}
};