#pragma once

#include "modifier.hpp"
#include "of.hpp"
#include "is_function.hpp"

namespace type {

	class add_pointer : type::modifier_marker {

		template<typename Type>
		struct add_pointer_t : type::of<Type*> {};

		template<typename Type>
		struct add_pointer_t<Type&> : type::of<Type*> {};

		template<typename Type>
		requires(::is_function<Type>)
		struct add_pointer_t<               Type&> : type::of<Type*> {};

		template<typename Type>
		requires(::is_function<Type>)
		struct add_pointer_t<const          Type&> : type::of<Type*> {};

		template<typename Type>
		requires(::is_function<Type>)
		struct add_pointer_t<const volatile Type&> : type::of<Type*> {};

		template<typename Type>
		requires(::is_function<Type>)
		struct add_pointer_t<      volatile Type&> : type::of<Type*> {};
	public:

		template<typename Type>
		using for_type = typename add_pointer_t<Type>::type;
	};

}

template<typename Type>
using add_pointer = type::add_pointer::for_type<Type>;