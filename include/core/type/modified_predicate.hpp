#pragma once

#include "predicate.hpp"
#include "modifier.hpp"

namespace type {

	template<type::predicate Predicate, type::modifier Modifier>
	struct modified_predicate {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = Predicate::template for_type_of<typename Modifier::template for_type_of<Type>>;
	};

}