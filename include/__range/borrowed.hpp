#pragma once

#include "./basic.hpp"
#include "../__type/is_derived_from.hpp"
#include "../__type/is_reference.hpp"


template<bool Value>
struct borrowed_range_mark {
	static constexpr bool is_borrowed_range = Value;
};

template<typename Type>
concept borrowed_range = basic_range<Type> && (
	(
		requires {
			{ Type::is_borrowed_range } -> same_as<const bool&>;
		}
		&& Type::is_borrowed_range
	)
	|| type_is_reference<Type>
);

static constexpr struct is_borrowed_range_t :
	type_predicate_extensions<is_borrowed_range_t>
{
	template<typename Type>
	constexpr bool for_type() const {
		return borrowed_range<Type>;
	}
} is_borrowed_range;