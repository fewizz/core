#pragma once

#include "count_of_decayed_same_as.hpp"

namespace types {

	template<typename Type>
	struct are_contain_decayed_same_as : types::count_of_decayed_same_as<Type>::template greater_or_equals<1>{};

	template<typename Type>
	struct are_contain_one_decayed_same_as : types::count_of_decayed_same_as<Type>::template equals<1>{};

	template<typename Type>
	struct are_may_contain_one_decayed_same_as : types::count_of_decayed_same_as<Type>::template lesss_or_equals<1>{};

}