#pragma once

namespace __type {

	template<typename Type, typename BaseType>
	constexpr inline bool is_same_as = __is_same_as(Type, BaseType);

}

template<typename Type, typename BaseType>
concept same_as = __is_same_as(Type, BaseType);

template<typename Type, typename BaseType>
concept not_same_as = !__is_same_as(Type, BaseType);; // genious