#pragma once

#include "integer.hpp"
#include "type/is_enum.hpp"
#include "types/are_same.hpp"

template<enum_type E>
struct flag_enum {
	using value_type = uint_of_size_of<E>;
	value_type value{};

	flag_enum() = default;
	flag_enum(const flag_enum&) = default;
	flag_enum(flag_enum&) = default;

	template<typename... Args>
	requires(
		sizeof...(Args) > 0
		&& types::are_same::for_types_of<Args..., E>
	)
	flag_enum(Args... args) {
		value = ((value_type) args | ...);
	}

	flag_enum& set(E v) {
		value |= (value_type) v;
		return *this;
	}

	bool get(E v) const {
		return (value | (value_type) v) == value;
	}
};