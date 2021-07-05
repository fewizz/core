#pragma once

#include <cinttypes>
#include <type_traits>
#include <limits>

namespace u {

constexpr std::size_t not_found = std::numeric_limits<std::size_t>::max();

namespace internal {

	template<typename T>
	constexpr std::size_t index_of(std::size_t index = 0) {
		return not_found;
	}

	template<typename T, typename Head, typename... Tail>
	constexpr std::size_t index_of(std::size_t index = 0) {
		if constexpr(std::is_same_v<T, Head>) return index;
		else return index_of<T, Tail...>(index + 1);
	}
}

template<typename T, typename... Ts>
constexpr std::size_t index_of = internal::index_of<T, Ts...>();

}