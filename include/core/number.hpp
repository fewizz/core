#pragma once

#include "wrapper/of_integer.hpp"
#include "meta/decayed_same_as.hpp"
#include "meta/type/is_invokable_with.hpp"
#include "meta/types/are_exclusively_satisfying_predicates.hpp"

struct base : wrapper::of_integer<nuint>{};
struct number : wrapper::of_integer<nuint>{};

template<typename... Args>
requires(
	types::are_exclusively_satisfying_predicates<
		types::are_contain_one_decayed_same_as<base>,
		types::are_contain_one_decayed_same_as<number>,
		types::are_contain_one_satisfying_predicate<type::is_invokable_with<nuint>>
	>::for_types<Args...>
)
void for_each_digit_in_number(Args... args) {
	nuint b = (nuint) elements::decayed_same_as<base>(args...);
	nuint n = (nuint) elements::decayed_same_as<number>(args...);
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

