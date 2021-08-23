#pragma once

#include "../parameter_pack/parameter_pack.hpp"
#include <tuple>
#include <utility>

namespace u {

template<typename... Ts, std::size_t... Indices>
constexpr auto get(std::tuple<Ts...> t, std::index_sequence<Indices...>) {
	return std::tuple( std::move(std::get<Indices>(t))... );
}

template<typename T, typename... Ts>
constexpr auto get(std::tuple<Ts...> t) {
	using PP = typename u::parameter_pack<Ts...>;
	return get<Ts...>(
		std::move(t),
		typename PP::template indices_of_same_as<T>{}
	);
}

} // u