#pragma once

#include "declaration.hpp"

#include "../meta/type/decay.hpp"
#include "../meta/type/predicate.hpp"

namespace type {

	class is_some_handle : ::type::predicate_marker {

		template<typename Type>
		static constexpr bool for_decayed_type = false;

		template<typename Type>
		static constexpr bool for_decayed_type<handle<Type>> = true;

	public:

		template<typename Type>
		static constexpr bool for_type = for_decayed_type<::decay<Type>>;

	};

} // type

template<typename Type>
concept some_handle = type::is_some_handle::for_type<Type>;