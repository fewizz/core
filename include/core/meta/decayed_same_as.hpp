#pragma once

#include "decayed_satisfying_predicate.hpp"

namespace type {

	template<typename Type>
	struct is_decayed_same_as : type::is_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	// alias

	template<typename Type>
	struct is_decayed : type::is_decayed_same_as<Type> {};

}

namespace types {

	template<typename Type>
	struct count_of_decayed_same_as : types::count_of_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	template<typename Type>
	struct indices_of_decayed_same_as : types::indices_of_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	template<typename Type>
	struct index_of_decayed_same_as : types::index_of_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	template<typename Type>
	struct are_contain_decayed_same_as : types::are_contain_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	template<typename Type>
	struct are_may_contain_decayed_same_as : types::are_may_contain_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	template<typename Type>
	struct are_contain_one_decayed_same_as : types::are_contain_one_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	template<typename Type>
	struct are_may_contain_one_decayed_same_as : types::are_may_contain_one_decayed_satisfying_predicate<type::is_same_as<Type>> {};

	// aliases

	template<typename Type>
	struct count_of_decayed : types::count_of_decayed_same_as<Type> {};

	template<typename Type>
	struct indices_of_decayed : types::indices_of_decayed_same_as<Type> {};

	template<typename Type>
	struct index_of_decayed : types::index_of_decayed_same_as<Type> {};

	template<typename Type>
	struct are_contain_decayed : types::are_contain_decayed_same_as<Type> {};

	template<typename Type>
	struct are_may_contain_decayed : types::are_may_contain_decayed_same_as<Type> {};

	template<typename Type>
	struct are_contain_one_decayed : types::are_contain_one_decayed_same_as<Type> {};

	template<typename Type>
	struct are_may_contain_one_decayed : types::are_may_contain_one_decayed_same_as<Type> {};

}

namespace elements {

	template<typename Type>
	inline constexpr auto decayed_same_as = elements::satisfying_predicate<
		type::is_decayed_same_as<Type>
	>;

	template<typename Type>
	inline constexpr auto for_each_decayed_same_as = elements::for_each_satisfying_type_predicate<
		type::is_decayed_same_as<Type>
	>;

	// aliases

	template<typename Type>
	inline constexpr auto decayed = elements::decayed_same_as<Type>;

	template<typename Type>
	inline constexpr auto for_each_decayed = elements::for_each_decayed_same_as<Type>;

}