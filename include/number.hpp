#pragma once

#include "./type.hpp"
#include "./types.hpp"

class number_base {
	const nuint value_;
public:
	number_base(nuint value) : value_{ value } {}
	operator nuint () const { return value_; }
};

template<unsigned_integer Number, typename Handler>
void for_each_digit_in_number(
	Number number, number_base base, Handler&& handler
) {
	nuint divisor = 1;
	Number number_copy = number;

	while((number /= base) > 0) {
		divisor *= base;
	}

	number = number_copy;

	while(divisor > 0) {
		auto d = (number / divisor) % base;
		handler(d);
		divisor /= base;
	}
}