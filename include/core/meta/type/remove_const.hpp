#pragma once

#include "of.hpp"
#include "modifier.hpp"

namespace type {

	class remove_const : type::modifier_marker {

		template<typename Type>
		struct remove_const_t : type::of<Type>{};

		template<typename Type>
		struct remove_const_t<const Type> : type::of<Type>{};

	public:

		template<typename Type>
		using for_type = typename remove_const_t<Type>::type;

	};

}

template<typename Type>
using remove_const = type::remove_const::for_type<Type>;