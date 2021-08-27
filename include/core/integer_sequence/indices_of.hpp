#pragma once

#include <utility>
#include "../index_sequence/append.hpp"

namespace u {

namespace internal {

	template<auto T, std::size_t Index, decltype(T)... Values>
	struct indices_of;

	template<auto T, std::size_t Index, decltype(T) T0, decltype(T)... Values>
	struct indices_of<T, Index, T0, Values...> {
		using type = typename indices_of<T, Index, Values...>::type;
	};

	template<auto T, std::size_t Index, decltype(T)... Values>
	struct indices_of<T, Index, T, Values...> {
		using type = u::append_back<
			Index,
			typename indices_of<T, Index, Values...>::type
		>;
	};
}

template<auto T, decltype(T)... Values>
constexpr auto indices_of() {
	
}

}