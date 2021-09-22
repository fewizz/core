#pragma once

#include "integer.hpp"
#include "meta/enum_type.hpp"
#include "meta/are_types_same.hpp"

template<enum_type E>
struct flag_enum {
	using value_type = unsigned_integer_of_size_of<E>;
	value_type value{};

	flag_enum() = default;
	flag_enum(const flag_enum&) = default;
	flag_enum(flag_enum&) = default;

	template<typename... Args>
	requires(
		sizeof...(Args) > 0
		&& are_same<Args..., E>
	)
	flag_enum(Args... args) {
		value = (value_type)(args | ...);
	}

	flag_enum& set(E v) {
		value |= (value_type)v;
		return *this;
	}

	bool get(E v) const {
		return (value | (value_type)v) == value;
	}
};