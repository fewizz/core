#pragma once

#include "./decayed_same_as.hpp"
#include "./type/is_invokable_with.hpp"
#include "./types/are_exclusively_satisfying_predicates.hpp"

class numer_base {
	nuint value_; 
public:

	numer_base(nuint value) : value_{ value } {}

	operator       nuint& ()       & { return value_; }
	operator const nuint& () const & { return value_; }
};

template<typename... Args>
requires(
	types::are_exclusively_satisfying_predicates<
		types::are_contain_one_decayed_same_as<numer_base>,
		types::are_contain_one_satisfying_predicate<
			type::is_integer
		>,
		types::are_contain_one_satisfying_predicate<
			type::is_invokable_with<nuint>
		>
	>::for_types<Args...>
)
void for_each_digit_in_number(Args... args) {
	auto b = elements::decayed_same_as<numer_base>(args...);
	auto n = elements::satisfying_predicate<
		type::is_integer
	>(args...);
	auto f = elements::satisfying_predicate<
		type::is_invokable_with<nuint>
	>(args...);

	nuint divisor = 1;
	nuint n_copy = n;

	while((n /= b) > 0) {
		divisor *= b;
	}

	n = n_copy;

	while(divisor > 0) {
		auto d = (n / divisor) % b;
		f(d);
		divisor /= b;
	}
}

