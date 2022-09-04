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

namespace __type {

	template<typename Type>
	using decay =
		typename if_satisfies<
			__type::is_array<__type::remove_reference<Type>>
		>::template then<
			__type::remove_extent<__type::remove_reference<Type>>
		>
		::template otherwise<
			typename if_satisfies<
				__type::is_function<__type::remove_reference<Type>>
			>
			::template then<
				__type::add_pointer<__type::remove_reference<Type>>
			>
			::template otherwise<
				__type::remove_volatile<
					__type::remove_const<__type::remove_reference<Type>>
				>
			>
		>;

}