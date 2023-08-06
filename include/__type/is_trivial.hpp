#pragma once

template<typename Type>
concept trivial = __is_trivial(Type);

namespace __type {

	template<typename Type>
	constexpr inline bool is_trivial = trivial<Type>;

}