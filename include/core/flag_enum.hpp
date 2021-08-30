#pragma once

#include <type_traits>
#include "integer.hpp"

template<typename E>
requires(std::is_enum_v<E>)
struct flag_enum {
	using value_type = int_with_size_of<E>;
	value_type value;

	flag_enum() = default;
	flag_enum(const flag_enum&) = default;
	flag_enum(flag_enum&) = default;

	template<typename... Args>
	requires(
		sizeof...(Args) > 0
		&& (std::is_same_v<Args, E> && ...)
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