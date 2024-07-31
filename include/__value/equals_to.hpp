#pragma once

#include "./predicate.hpp"

template<auto Value>
struct equals_to_t : value_predicate_mark {

	template<auto EqualValue>
	constexpr bool for_value() const {
		return Value == EqualValue;
	}

};

template<auto Value>
struct equals_to_t<Value> equals_to{};