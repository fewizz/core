#pragma once

#include "elements/satisfying_predicate.hpp"
#include "integer.hpp"
#include "types/count_of_satisfying_predicate.hpp"
#include "wrapper/of_integer.hpp"
#include "types/are_exclusively_satsify_predicates.hpp"
#include "types/count_of_type.hpp"
#include "elements/of_type.hpp"
#include "type/is_invokable_with.hpp"

struct base : wrapper::of_integer<nuint>{};
struct number : wrapper::of_integer<nuint>{};

template<typename... Args>
requires(
	types::are_exclusively_satsify_predicates<
		types::count_of_type<base>::equals<1>,
		types::count_of_type<number>::equals<1>,
		types::count_of_satisfying_predicate<type::is_invokable_with<nuint>>::equals<1>
	>::for_types_of<Args...>
)
void for_each_digit_in_number(Args... args) {
	nuint b = (nuint) elements::of_type<base&>::for_elements_of(args...);
	nuint n = (nuint) elements::of_type<number&>::for_elements_of(args...);
	auto f = elements::satisfying_predicate<type::is_invokable_with<nuint>>::for_elements_of(args...);

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

