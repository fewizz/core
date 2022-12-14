#pragma once

namespace __type {

	template<typename Type, typename OtherType>
	constexpr inline bool is_same_as = __is_same_as(Type, OtherType);

}

template<typename Type, typename OtherType>
concept same_as = __is_same_as(Type, OtherType);

template<typename Type, typename... OtherTypes>
concept same_as_any = (same_as<Type, OtherTypes> || ...);

template<typename Type, typename OtherType>
concept not_same_as = !same_as<Type, OtherType>; // genious