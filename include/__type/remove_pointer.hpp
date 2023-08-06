#pragma once

namespace __type {

	template<typename Type>
	struct remove_pointer_t        { using type = Type; };

	template<typename Type>
	struct remove_pointer_t<Type*> { using type = Type; };

}

template<typename Type>
using remove_pointer = typename __type::remove_pointer_t<Type>::type;