#pragma once

#include "predicate.hpp"
#include "remove_reference.hpp"

namespace type {
	
	template<type::predicate Predicate>
	struct ignore_reference {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = Predicate::template for_type_of<remove_reference::for_type_of<Type>>;
	};
}