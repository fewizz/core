#pragma once

#include <tuple>
#include "at.hpp"
#include "indices_of.hpp"
#include "types.hpp"
#include "values.hpp"

template<typename... Ts>
struct tuple : std::tuple<Ts...> {
	static constexpr std::size_t size = sizeof...(Ts);
	using indices_type = typename indices::from<0>::to<size>;
	using types_type = types::of<Ts...>;

	template<typename T>
	static constexpr bool contains = types_type::template contains<T>;

	template<typename T>
	static constexpr auto count = types_type::template count<T>;

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
	decltype(auto) move_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ move_element_at<Indices>() ... };
	}

	template<typename T>
	decltype(auto) move_element() {
		using IndicesOfSameAsT = typename types_type::template indices_of_same_as<T>;
		static_assert(IndicesOfSameAsT::size == 1);
		return move_element_at<IndicesOfSameAsT::template value_at<0>>();
	}

	template<typename T>
	decltype(auto) consume_element(auto&& f) {
		using IndicesOfSameAsT = typename types_type::template indices_of_same_as<T>;
		static_assert(IndicesOfSameAsT::size == 1);
		constexpr std::size_t I = IndicesOfSameAsT::template value_at<0>;

		f(move_element_at<I>());
		return erase_element_at<I>();
	}

	template<typename T, typename F>
	decltype(auto) consume_element_if_present(F&& f) {
		if constexpr(types_type::template contains<T>) {
			return consume_element<T>(std::forward<F>(f));
		}
		else {
			return std::move(*this);
		}
	}

	template<typename T>
	using indices_of_same_as = typename types_type::template indices_of_same_as<T>;

	template<typename T>
	auto move_elements_same_as() {
		return move_elements_at(indices_of_same_as<T>{});
	}

	template<typename T>
	auto erase_elements_same_as() {
		return erase_elements_at(indices_of_same_as<T>{});
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
		return move_elements_at(typename indices_type::template erase_value_at<Index>{});
	}

	template<std::size_t... Indices>
	auto erase_elements_at(indices::of<Indices...> = {}) {
		using Indices0 = typename indices_type::template erase_values_at<Indices...>;
		return move_elements_at(Indices0{});
	}


};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;