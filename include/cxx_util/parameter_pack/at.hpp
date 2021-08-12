#pragma once

#include <cstddef>

namespace u {

namespace internal {

	template<std::size_t index, typename... Ts>
	struct at;

	template<typename T, typename... Tail>
	struct at<0, T, Tail...> { using type = T; };

	template<std::size_t Index, typename Head, typename... Tail>
	struct at<Index, Head, Tail...> {
		using type = typename at<Index - 1, Tail...>::type;
	};
}

template<std::size_t Index, typename... Ts>
using at = typename internal::at<Index, Ts...>::type;

}