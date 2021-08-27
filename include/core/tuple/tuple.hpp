#pragma once

#include <__utility/integer_sequence.h>
#include <utility>
#include <tuple>
#include "../parameter_pack/at.hpp"
#include "parameter_pack/parameter_pack.hpp"
#include "index_sequence.hpp/erase.hpp"

namespace u {

template<typename... Ts>
struct tuple : std::tuple<Ts...> {
	static constexpr std::size_t size = sizeof...(Ts);

	tuple(Ts&&... ts)
		: std::tuple<Ts...> { std::forward<Ts>(ts) ... }
	{}

	operator std::tuple<Ts...>& () { return *this; }
	operator const std::tuple<Ts...>& () const { return *this; }

	template<std::size_t Index>
	decltype(auto) move_element() {
		return std::get<Index>(std::move(*this));
	}

	template<std::size_t... Indices>
	requires(sizeof...(Indices) > 0)
	auto move(std::index_sequence<Indices...> = {}) {
		return u::tuple{ move_element<Indices>() ... };
	}

	template<std::size_t Index>
	decltype(auto) element_at() const {
		return std::get<Index>(*this);
	}

	template<std::size_t... Indices>
	auto at(std::index_sequence<Indices...> = {}) {
		return u::tuple{ element_at<Indices>() ... };
	}

	template<std::size_t Index>
	auto erase_element() {
		return move(u::erase_at<Index>(std::index_sequence_for<Ts...>{}));
	}

	template<std::size_t Index>
	auto erase() {
		return move(u::erase_at<Index>(std::index_sequence_for<Ts...>{}));
	}
};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;

} // u