#pragma once

#include <cinttypes>

namespace u {

namespace internal {

	template<std::size_t index, typename... Ts>
	struct at;

	template<std::size_t index, typename Head, typename... Tail>
	struct at<index, Head, Tail...> {
		using type = typename at<index - 1, Tail...>::type;
	};

	template<typename T, typename... Tail>
	struct at<0, T, Tail...> { using type = T; };

	template<typename T>
	struct at<0, T> { static_assert(true, "out of bounds"); };

}

template<std::size_t Index, typename... Ts>
using at = typename internal::at<Index, Ts...>::type;

}