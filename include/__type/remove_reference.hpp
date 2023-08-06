#pragma once

namespace __type {

	template<typename Type>
	struct remove_reference_t         { using type = Type; };

	template<typename Type>
	struct remove_reference_t<Type&>  { using type = Type; };

	template<typename Type>
	struct remove_reference_t<Type&&> { using type = Type; };

}

template<typename Type>
using remove_reference = typename __type::remove_reference_t<Type>::type;