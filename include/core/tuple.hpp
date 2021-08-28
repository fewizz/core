#pragma once

#include <tuple>
#include "at.hpp"
#include "indices_of.hpp"
#include "types.hpp"
#include "indices.hpp"

template<typename... Ts>
struct tuple : std::tuple<Ts...> {
	static constexpr std::size_t size = sizeof...(Ts);

	tuple(Ts&&... ts)
		: std::tuple<Ts...> { std::forward<Ts>(ts) ... }
	{}

	operator std::tuple<Ts...>& () { return *this; }
	operator const std::tuple<Ts...>& () const { return *this; }

	template<std::size_t Index>
	decltype(auto) move_element_at() {
		return std::get<Index>(std::move(*this));
	}

	template<std::size_t... Indices>
	requires(sizeof...(Indices) > 0)
	auto move_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ move_element_at<Indices>() ... };
	}

	template<std::size_t Index>
	decltype(auto) element_at() const {
		return std::get<Index>(*this);
	}

	template<std::size_t... Indices>
	auto elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ element_at<Indices>() ... };
	}

	template<std::size_t Index>
	auto erase_element_at() {
		using Indices = typename indices::from<0>::template to<size>;
		return move_elements_at(typename Indices::template erase_at_index<Index>{});
	}

	template<std::size_t... Indices>
	auto erase_elements_at() {
		using CurrentIndices = typename indices::template from<0>::template to<size>;
		
		//using NagatedIndices = indices::of_values_that_not_satisfy<CurrentIndices::template contains_index>
		//return move_elements_at(typename Indices::template erase_at_index<Index>{});
	}
};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;