#pragma once

#include <type_traits>

namespace u {

template<typename... Ts>
struct are {

	template<typename T>
	using args_for_invocable = std::is_invocable<T, Ts...>;
};

}