#pragma once

namespace __type {

	template<typename Type>
	struct remove_const_t             { using type = Type; };

	template<typename Type>
	struct remove_const_t<const Type> { using type = Type; };

	template<typename Type>
	using remove_const = typename remove_const_t<Type>::type;

}