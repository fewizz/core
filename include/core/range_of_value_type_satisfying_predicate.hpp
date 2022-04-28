#pragma once

#include "range.hpp"
#include "value_type.hpp"

template<typename Range, typename Predicate>
concept range_of_value_type_satisfying_predicate =
	range<Range> && type::predicate<Predicate> &&
	Predicate::template for_type<value_type<Range>>;

namespace type {

	template<type::predicate Predicate>
	struct is_range_of_value_type_satisfying_predicate :
		type::predicate_marker 
	{

		template<typename Range>
		static constexpr bool for_type =
			range_of_value_type_satisfying_predicate<Range, Predicate>;

	};

} // type

#include "core/meta/types/count_of_satisfying_predicate.hpp"
#include "core/meta/types/are_contain_satisfying_predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	struct count_of_ranges_of_value_type_satisfying_predicate :
		types::count_of_satisfying_predicate<
			type::is_range_of_value_type_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_contain_ranges_of_value_type_satisfying_predicate :
		types::are_contain_satisfying_predicate<
			type::is_range_of_value_type_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_may_contain_ranges_of_value_type_satisfying_predicate :
		types::are_may_contain_satisfying_predicate<
			type::is_range_of_value_type_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_contain_range_of_value_type_satisfying_predicate :
		types::are_contain_one_satisfying_predicate<
			type::is_range_of_value_type_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_may_contain_range_of_value_type_satisfying_predicate :
		types::are_may_contain_one_satisfying_predicate<
			type::is_range_of_value_type_satisfying_predicate<Predicate>
		>
	{};

} // types

#include "core/meta/elements/satisfying_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	inline constexpr auto range_of_value_type_satisfying_predicate =
		elements::satisfying_predicate<
			type::is_range_of_value_type_satisfying_predicate<Predicate>
		>;

} // elements