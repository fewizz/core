#pragma once

#include "begin.hpp"
#include "end.hpp"
#include "meta/type/remove_const.hpp"
#include "meta/type/is_same_as.hpp"
#include "meta/type/remove_reference.hpp"

template<typename T>
concept range = requires(T& t) {
	begin(t);
	end(t);
};

namespace type {

	struct is_range : type::predicate_marker {
		
		template<typename Type>
		static constexpr bool for_type = range<Type>;

	};

} // type

#include "meta/types/are_contain_satisfying_predicate.hpp"

namespace types {

	struct count_of_ranges :
		types::count_of_satisfying_predicate<type::is_range>
	{};

	struct are_contain_ranges :
		types::are_contain_satisfying_predicate<type::is_range>
	{};

	struct are_may_contain_ranges :
		types::are_may_contain_satisfying_predicate<type::is_range>
	{};

	struct are_contain_range :
		types::are_contain_one_satisfying_predicate<type::is_range>
	{};

	struct are_may_contain_range :
		types::are_may_contain_one_satisfying_predicate<type::is_range>
	{};

} // types

#include "meta/elements/satisfying_predicate.hpp"

namespace elements {

	inline constexpr auto range =
		elements::satisfying_predicate<type::is_range>;

} // elements