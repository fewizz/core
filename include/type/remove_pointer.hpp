#pragma once

#include "modifier.hpp"
#include "../integer.hpp"

namespace type {

	class remove_pointer : type::modifier_marker {

		template<typename Type>
		struct remove_pointer_t : type::of<Type> {};

		template<typename Type>
		struct remove_pointer_t<Type*> : type::of<Type> {};

	public:

		template<typename Type>
		using for_type = typename remove_pointer_t<Type>::type;

	};

} // type

template<typename Type>
using remove_pointer = type::remove_pointer::for_type<Type>;