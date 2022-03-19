#pragma once

#include "some.hpp"
#include "some_guarded.hpp"

template<typename Type>
concept some_possibly_guarded_handle = some_handle<Type> || some_guarded_handle<Type>;

namespace type {

	struct is_some_possibly_guarded_handle : ::type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = some_possibly_guarded_handle<Type>;

	};

} // type