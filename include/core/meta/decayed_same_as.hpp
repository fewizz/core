#pragma once

#include "type/is_decayed_same_as.hpp"
#include "types/count_of_satisfying_predicate.hpp"
#include "types/index_of_satisfying_predicate.hpp"
#include "types/are_contain_satisfying_predicate.hpp"

namespace types {

	template<typename Type>
	struct count_of_decayed_same_as : types::count_of_satisfying_predicate<type::is_decayed_same_as<Type>> {};

	template<typename Type>
	struct indices_of_type : types::indices_of_satisfying_predicate<type::is_decayed_same_as<Type>> {};

	template<typename Type>
	struct index_of_decayed_same_as : types::index_of_satisfying_predicate<type::is_decayed_same_as<Type>> {};

	template<typename Type>
	struct are_contain_decayed_same_as : types::are_contain_satisfying_predicate<type::is_decayed_same_as<Type>> {};

	template<typename Type>
	struct are_may_contain_decayed_same_as : types::are_may_contain_satisfying_predicate<type::is_decayed_same_as<Type>> {};

	template<typename Type>
	struct are_contain_one_decayed_same_as : types::are_contain_one_satisfying_predicate<type::is_decayed_same_as<Type>> {};

	template<typename Type>
	struct are_may_contain_one_decayed_same_as : types::are_may_contain_one_satisfying_predicate<type::is_decayed_same_as<Type>> {};

}

#include "elements/satisfying_predicate.hpp"
#include "elements/for_each_satisfying_type_predicate.hpp"

namespace elements {

	template<typename Type>
	inline constexpr auto decayed_same_as = elements::satisfying_predicate<
		type::is_decayed_same_as<Type>
	>;

	template<typename Type>
	inline constexpr auto for_each_decayed_same_as = elements::for_each_satisfying_type_predicate<
		type::is_decayed_same_as<Type>
	>;

}