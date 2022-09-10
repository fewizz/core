#pragma once

#include "./type.hpp"
#include "./types.hpp"

class number_base {
	nuint value_; 
public:

	number_base(nuint value) : value_{ value } {}

	operator       nuint& ()       & { return value_; }
	operator const nuint& () const & { return value_; }
};

template<integer Number, typename Handler>
void for_each_digit_in_number(
	Number number, number_base base, Handler&& handler
) {
	nuint divisor = 1;
	nuint number_copy = number;

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