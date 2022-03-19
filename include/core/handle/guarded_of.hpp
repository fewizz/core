#pragma once

#include "guarded_declaration.hpp"

#include "../meta/type/is_same_as.hpp"
#include "../meta/type/decay.hpp"

template<typename Type, typename ObjectType>
concept guarded_handle_of = type::is_same_as<guarded_handle<ObjectType>>::template for_type<decay<Type>>;

namespace type {

	template<typename ObjectType>
	struct is_guarded_handle_of : ::type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = guarded_handle_of<Type, ObjectType>;

	};

} // type