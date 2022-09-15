#pragma once

#include "./integer.hpp"
#include "./type.hpp"
#include "./types.hpp"

struct enum_flags_iterator {
	
};

template<typename Enum>
requires type<Enum>::is_enum
class enum_flags {
	using value_type = uint_of_size_of<Enum>;
	value_type value_{};
public:

	template<typename... Args>
	requires(sizeof...(Args) == 0)
	constexpr enum_flags(Args...) {}

	template<typename... Args>
	requires(sizeof...(Args) > 0 && types_are_same<Enum, Args...>)
	constexpr enum_flags(Args... args) {
		value_ = ((value_type) args | ...);
	}

	constexpr enum_flags& set(Enum v) {
		value_ |= (value_type) v;
		return *this;
	}

	constexpr value_type set_count() const
	requires(sizeof(value_type) <= sizeof(unsigned long long)) {
		if constexpr(sizeof(value_type) == sizeof(unsigned)) {
			return __builtin_popcount(value_);
		} else
		if constexpr(sizeof(value_type) == sizeof(unsigned long)) {
			return __builtin_popcountl(value_);
		} else
		if constexpr(sizeof(value_type) == sizeof(unsigned long long)) {
			return __builtin_popcountll(value_);
		}
	}

	constexpr bool get(Enum v) const {
		bool result = (value_ | (value_type) v) == value_;
		return result;
	}

	constexpr bool at(value_type index) const {
		return (value_ >> index) & 1;
	}

	constexpr enum_flags operator & (enum_flags other) const {
		return { (Enum) (value_ & other.value_) };
	}

	constexpr operator value_type () const { return value_; }

}; // flag_enum