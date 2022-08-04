#pragma once

#include "decayed_same_as.hpp"
#include "type/is_invokable_with.hpp"
#include "types/are_exclusively_satisfying_predicates.hpp"

struct base { nuint _; operator nuint () const { return _; }};
struct number { nuint _; operator nuint () const { return _; }};

template<typename... Args>
requires(
	types::are_exclusively_satisfying_predicates<
		types::are_contain_one_decayed_same_as<base>,
		types::are_contain_one_decayed_same_as<number>,
		types::are_contain_one_satisfying_predicate<type::is_invokable_with<nuint>>
	>::for_types<Args...>
)
void for_each_digit_in_number(Args... args) {
	auto b = elements::decayed_same_as<base>(args...);
	auto n = elements::decayed_same_as<number>(args...);
	auto f = elements::satisfying_predicate<type::is_invokable_with<nuint>>(args...);

	nuint to_divide = 1;
	nuint n_copy = n;

	while((n_copy /= b) > 0) {
		to_divide *= b;
	}

	while(to_divide > 0) {
		auto d = (n / to_divide) % b;
		f(d);
		to_divide /= b;
	}
}

