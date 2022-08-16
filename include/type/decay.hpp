#pragma once

#include "../if_satisfies.hpp"
#include "./remove_reference.hpp"
#include "./remove_extent.hpp"
#include "./remove_volatile.hpp"
#include "./remove_const.hpp"
#include "./add_pointer.hpp"
#include "./add_pointer.hpp"
#include "./is_function.hpp"
#include "./is_array.hpp"

namespace type {

	struct decay : type::modifier_marker {

		template<typename Type>
		using for_type =
			typename if_satisfies<
				::is_array<::remove_reference<Type>>
			>::template then<
				::remove_extent<::remove_reference<Type>>
			>
			::template otherwise<
				typename if_satisfies<
					::is_function<::remove_reference<Type>>
				>
				::template then<
					::add_pointer<::remove_reference<Type>>
				>
				::template otherwise<
					::remove_volatile<::remove_const<::remove_reference<Type>>>
				>
			>;

	};

} // type

template<typename Type>
using decay = type::decay::for_type<Type>;