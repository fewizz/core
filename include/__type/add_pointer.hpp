#pragma once

#include "./is_function.hpp"

namespace __type {

	template<typename Type>
	struct add_pointer_t                       { using type = Type*; };

	template<typename Type>
	struct add_pointer_t<Type&>                { using type = Type*; };

	template<typename Type>
	requires __type::is_function<Type>
	struct add_pointer_t<Type&>                { using type = Type*; };

	template<typename Type>
	requires __type::is_function<Type>
	struct add_pointer_t<const Type&>          { using type = Type*; };

	template<typename Type>
	requires __type::is_function<Type>
	struct add_pointer_t<const volatile Type&> { using type = Type*; };

	template<typename Type>
	requires __type::is_function<Type>
	struct add_pointer_t<      volatile Type&> { using type = Type*; };

	template<typename Type>
	using add_pointer = typename add_pointer_t<Type>::type;

};