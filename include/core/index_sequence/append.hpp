#pragma once

#include <__utility/integer_sequence.h>
#include <utility>

namespace u {

/*template<typename A, typename B>
struct append_indices;

template<std::size_t... A, std::size_t... B>
struct append_indices<std::index_sequence<A...>, std::index_sequence<B...>> {
	using type = std::index_sequence<A..., B...>;

	auto operator () (std::index_sequence<A...>, std::index_sequence<B...>) {
		return type{};
	}

};*/

template<std::size_t V, std::size_t... Indices>
using append_front = std::index_sequence<V, Indices...>;

template<std::size_t V, std::size_t... Indices>
using append_back = std::index_sequence<Indices..., V>;

};