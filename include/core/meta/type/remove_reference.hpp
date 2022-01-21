#pragma once

#include "of.hpp"
#include "modifier.hpp"

namespace type {

	class remove_reference : type::modifier_marker {

		template<typename Type>
		struct remove_reference_t : type::of<Type> {};

		template<typename Type>
		struct remove_reference_t<Type&> : type::of<Type> {};

		template<typename Type>
		struct remove_reference_t<Type&&> : type::of<Type> {};

	public:

		template<typename Type>
		using for_type = typename remove_reference_t<Type>::type;

	};

}

template<typename Type>
using remove_reference = type::remove_reference::for_type<Type>;