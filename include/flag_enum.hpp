#pragma once

#include "integer.hpp"
#include "type/is_enum.hpp"
#include "types/are_same.hpp"

template<typename Enum>
requires type::is_enum::for_type<Enum>
struct flag_enum {
	using value_type = uint_of_size_of<Enum>;
	value_type value{};

	template<typename... Args>
	requires(sizeof...(Args) == 0)
	constexpr flag_enum(Args...) {}

	template<typename... Args>
	requires(sizeof...(Args) > 0 && types_are_same<Enum, Args...>)
	constexpr flag_enum(Args... args) {
		value = ((value_type) args | ...);
	}

	constexpr flag_enum& set(Enum v) {
		value |= (value_type) v;
		return *this;
	}

	constexpr bool get(Enum v) const {
		return (value | (value_type) v) == value;
	}

	constexpr bool at(value_type index) const {
		return (value >> index) & 1;
	}

	constexpr flag_enum operator & (flag_enum other) const {
		return { (Enum) (value & other.value) };
	}

}; // flag_enum