#pragma once

#include "./type/is_array.hpp"
#include "./type/remove_const.hpp"
#include "./type/is_same_as.hpp"
#include "./type/remove_reference.hpp"
#include "../__iterator/basic.hpp"

template<typename Type>
concept basic_range =
	(
		requires(Type& o) { { o.iterator()  } -> basic_iterator; } &&
		requires(Type& o) { o.sentinel(); }
	) ||
	is_array<Type>;

namespace type {

	struct is_basic_range : type::predicate_marker {
		
		template<typename Type>
		static constexpr bool for_type = basic_range<Type>;

	};

} // type

#include "./types/are_contain_satisfying_predicate.hpp"

namespace types {

	struct count_of_ranges :
		types::count_of_satisfying_predicate<type::is_basic_range>
	{};

	struct are_contain_ranges :
		types::are_contain_satisfying_predicate<type::is_basic_range>
	{};

	struct are_may_contain_ranges :
		types::are_may_contain_satisfying_predicate<type::is_basic_range>
	{};

	struct are_contain_range :
		types::are_contain_one_satisfying_predicate<type::is_basic_range>
	{};

	struct are_may_contain_range :
		types::are_may_contain_one_satisfying_predicate<type::is_basic_range>
	{};

} // types

#include "./elements/satisfying_predicate.hpp"

namespace elements {

	inline constexpr auto range =
		elements::satisfying_predicate<type::is_basic_range>;

} // elements