#pragma once

#include "./integer.hpp"
#include "./type.hpp"
#include "./types/are_same.hpp"

template<typename Enum>
requires type<Enum>::is_enum
class enum_flags {
	using value_type = uint_of_size_of<Enum>;
	value_type value{};
public:

	template<typename... Args>
	requires(sizeof...(Args) == 0)
	constexpr enum_flags(Args...) {}

	template<typename... Args>
	requires(sizeof...(Args) > 0 && types_are_same<Enum, Args...>)
	constexpr enum_flags(Args... args) {
		value = ((value_type) args | ...);
	}

	constexpr enum_flags& set(Enum v) {
		value |= (value_type) v;
		return *this;
	}

	constexpr bool get(Enum v) const {
		bool result = (value | (value_type) v) == value;
		return result;
	}

	constexpr bool at(value_type index) const {
		return (value >> index) & 1;
	}

	constexpr enum_flags operator & (enum_flags other) const {
		return { (Enum) (value & other.value) };
	}

	constexpr operator value_type () const { return value; }

}; // flag_enum