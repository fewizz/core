#pragma once

#include "declaration.hpp"

#include "../meta/type/decay.hpp"
#include "../meta/type/is_same_as.hpp"

template<typename Type, typename ObjectType>
concept handle_of =
	same_as<Type, handle<ObjectType>> &&
	requires(Type& o) {
		o.value() -> same_as<ObjectType>;
	};

namespace type {

	template<typename ObjectType>
	struct is_handle_of : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = handle_of<Type, ObjectType>;

	};

} // type