#pragma once

#include "../meta/type/remove_const.hpp"
#include "../meta/type/is_same_as.hpp"
#include "../meta/type/remove_reference.hpp"

namespace range {

	template<typename T>
	concept basic = requires(T t) {
		t.begin();
		t.end();
		typename remove_reference<remove_const<T>>::value_type;
	};

} // range

namespace type {

	struct is_basic_range : type::predicate_marker {
		
		template<typename Type>
		static constexpr bool for_type = range::basic<Type>;

	};

} // type

#include "../meta/types/are_contain_satisfying_predicate.hpp"

namespace types {

	struct count_of_basic_ranges :
		types::count_of_satisfying_predicate<type::is_basic_range>
	{};

	struct are_contain_basic_ranges :
		types::are_contain_satisfying_predicate<type::is_basic_range>
	{};

	struct are_may_contain_basic_ranges :
		types::are_may_contain_satisfying_predicate<type::is_basic_range>
	{};

	struct are_contain_basic_range :
		types::are_contain_one_satisfying_predicate<type::is_basic_range>
	{};

	struct are_may_contain_basic_range :
		types::are_may_contain_one_satisfying_predicate<type::is_basic_range>
	{};

} // types

#include "../meta/elements/satisfying_predicate.hpp"

namespace elements {

	inline constexpr auto basic_range =
		elements::satisfying_predicate<type::is_basic_range>;

} // elements