#pragma once

#include "../types/are_same.hpp"
#include "remove_reference.hpp"
#include "remove_const.hpp"
#include "modified_predicate.hpp"
#include "negated_predicate.hpp"

namespace type {

	template<typename BaseType>
	struct is_same_as : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = types::are_same::for_types_of<BaseType, Type>;

	};

}