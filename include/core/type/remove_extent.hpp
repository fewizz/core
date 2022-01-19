#pragma once

#include "modifier.hpp"
#include "../integer.hpp"

namespace type {

	struct remove_extent : type::modifier_marker {

		template<typename Type>
		struct remove_extent_t : type::of<Type> {};

		template<typename Type, nuint N>
		struct remove_extent_t<Type[N]> : type::of<Type> {};

		template<typename Type>
		using for_type = typename remove_extent_t<Type>::type;
	};

}

template<typename Type>
using remove_extent = type::remove_extent::for_type<Type>;