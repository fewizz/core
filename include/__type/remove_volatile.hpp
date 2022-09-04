#pragma once

namespace __type {

	template<typename Type>
	struct remove_volatile_t             { using type = Type; };

	template<typename Type>
	struct remove_volatile_t<const Type> { using type = Type; };

	template<typename Type>
	using remove_volatile = typename remove_volatile_t<Type>::type;

}