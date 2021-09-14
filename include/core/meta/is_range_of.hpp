#pragma once

#include <ranges>
#include <type_traits>
#include "is_same.hpp"

template<typename Type>
struct is_range {

	template<typename ValueType>
	static constexpr bool of =
		std::ranges::range<Type> &&
		is_same<
			std::ranges::range_value_t<Type>
		>::template as<ValueType>;

	template<typename ValueType>
	using of_predicate = std::bool_constant<of<ValueType>>;
};