#pragma once

#include "../parameter_pack/indices_of.hpp"
#include <tuple>
#include <utility>

namespace u {

namespace internal {

template<typename T, typename... Ts, std::size_t... Indices>
constexpr auto get(std::tuple<Ts...>& t, std::index_sequence<Indices...>) {
	return std::tuple{ std::get<Indices>(t)... };
}

} // internal

template<typename T, typename... Ts>
constexpr auto get(std::tuple<Ts...>& t) {
	return internal::get(t, u::indices_of<T, Ts...>{});
}

} // u