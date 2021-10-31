#pragma once

#include "elements/satisfying_predicate.hpp"
#include "integer.hpp"
#include "types/count_of_satisfying_predicate.hpp"
#include "wrapper/of_integer.hpp"
#include "types/are_exclusively_satsify_predicates.hpp"
#include "types/count_of_type.hpp"
#include "elements/of_type.hpp"
#include "type/is_invokable_with.hpp"

struct base : wrapper::of_integer<uint>{};
struct number : wrapper::of_integer<uint>{};

template<typename... Args>
requires(
	types::are_exclusively_satsify_predicates<
		types::count_of_type<base>::equals<1>,
		types::count_of_type<number>::equals<1>,
		types::count_of_satisfying_predicate<type::is_invokable_with<uint>>::equals<1>
	>::for_types_of<Args...>
)
void for_each_digit_in_number(Args... args) {
	uint b = (uint) elements::of_type<base&>::for_elements_of(args...);
	uint n = (uint) elements::of_type<number&>::for_elements_of(args...);
	auto f = elements::satisfying_predicate<type::is_invokable_with<uint>>::for_elements_of(args...);

	uint to_divide = 1;
	uint n_copy = n;

	while((n_copy /= b) > 0) {
		to_divide *= b;
	}

	while(to_divide > 0) {
		auto d = (n / to_divide) % b;
		f(d);
		to_divide /= b;
	}
}

