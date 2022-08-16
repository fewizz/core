#pragma once

#include "./type/predicate.hpp"
#include "./types/count_of_satisfying_predicate.hpp"
#include "./types/index_of_satisfying_predicate.hpp"
#include "./types/are_contain_satisfying_predicate.hpp"

namespace type {

	template<type::predicate Predicate>
	struct is_decayed_satisfying_predicate :
		type::predicate_marker
	{

		template<typename Type>
		static constexpr bool for_type =
			Predicate::template for_type<::decay<Type>>;

	};

} // type

namespace types {

	template<type::predicate Predicate>
	struct count_of_decayed_satisfying_predicate :
		types::count_of_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct indices_of_decayed_satisfying_predicate :
		types::indices_of_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct index_of_decayed_satisfying_predicate :
		types::index_of_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_contain_decayed_satisfying_predicate :
		types::are_contain_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_may_contain_decayed_satisfying_predicate :
		types::are_may_contain_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_contain_one_decayed_satisfying_predicate :
		types::are_contain_one_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

	template<type::predicate Predicate>
	struct are_may_contain_one_decayed_satisfying_predicate :
		types::are_may_contain_one_satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>
	{};

} // types

#include "elements/satisfying_predicate.hpp"
#include "elements/for_each_satisfying_type_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	inline constexpr auto decayed_satisfying_predicate =
		elements::satisfying_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>;

	template<type::predicate Predicate>
	inline constexpr auto for_each_decayed_satisfying_predicate =
		elements::for_each_satisfying_type_predicate<
			type::is_decayed_satisfying_predicate<Predicate>
		>;

} // elements