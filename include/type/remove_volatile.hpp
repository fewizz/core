#pragma once

#include "of.hpp"
#include "modifier.hpp"

namespace type {

	class remove_volatile : type::modifier_marker {

		template<typename Type>
		struct remove_volatile_t : type::of<Type>{};

		template<typename Type>
		struct remove_volatile_t<const Type> : type::of<Type>{};

	public:

		template<typename Type>
		using for_type = typename remove_volatile_t<Type>::type;

	};

} // type

template<typename Type>
using remove_volatile = type::remove_volatile::for_type<Type>;