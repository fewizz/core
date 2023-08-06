#pragma once

#include "../if_satisfies.hpp"
#include "./remove_reference.hpp"
#include "./remove_extent.hpp"
#include "./remove_volatile.hpp"
#include "./remove_const.hpp"
#include "./add_pointer.hpp"
#include "./is_function.hpp"
#include "./is_array.hpp"

template<typename Type>
using decay =
	typename if_satisfies<
		type_is_array<remove_reference<Type>>
	>::template then<
		remove_extent<remove_reference<Type>>
	>
	::template otherwise<
		typename if_satisfies<
			__type::is_function<remove_reference<Type>>
		>
		::template then<
			__type::add_pointer<remove_reference<Type>>
		>
		::template otherwise<
			remove_volatile<
				remove_const<remove_reference<Type>>
			>
		>
	>;