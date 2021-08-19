#pragma once

#include <type_traits>
#include "int_with_size.hpp"

namespace u {

template<typename E>
requires(std::is_enum_v<E>)
struct bitmask_from_enum {
	using value_type = u::int_with_size_of<E>;
	value_type value;

	bitmask_from_enum() = default;
	bitmask_from_enum(const bitmask_from_enum&) = default;
	bitmask_from_enum(bitmask_from_enum&) = default;

	template<typename... Args>
	requires(
		sizeof...(Args) > 0
		&& (std::is_same_v<Args, E> && ...)
	)
	bitmask_from_enum(Args... args) {
		value = (value_type)(args | ...);
	}

	bitmask_from_enum& set(E v) {
		value |= (value_type)v;
		return *this;
	}

	bool get(E v) const {
		return (value | (value_type)v) == value;
	}
};

}