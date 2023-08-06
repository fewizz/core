#pragma once

namespace __type {

	template<typename Type>
	struct remove_extent_t          { using type = Type; };

	template<typename Type, auto N>
	struct remove_extent_t<Type[N]> { using type = Type; };

}

template<typename Type>
using remove_extent = typename __type::remove_extent_t<Type>::type;