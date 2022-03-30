#pragma once

#include "count_of_satisfying_predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	struct are_contain_satisfying_predicate :
		types::count_of_satisfying_predicate<Predicate>::template
		greater<0>
	{};

	template<type::predicate Predicate>
	struct are_may_contain_satisfying_predicate :
		types::count_of_satisfying_predicate<Predicate>::template
		greater_or_equals<0>
	{};

	template<type::predicate Predicate>
	struct are_contain_one_satisfying_predicate :
		types::count_of_satisfying_predicate<Predicate>::template
		equals<1>
	{};

	template<type::predicate Predicate>
	struct are_may_contain_one_satisfying_predicate :
		types::count_of_satisfying_predicate<Predicate>::template
		less_or_equals<1>
	{};

} // types