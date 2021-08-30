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
	static constexpr bool contains_element = types_type::template contains_type<T>;

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

	template<std::size_t Index>
	decltype(auto) get_element_at() {
		return std::get<Index>(*this);
	}

	template<std::size_t... Indices>
	decltype(auto) move_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ move_element_at<Indices>() ... };
	}

	template<typename T>
	decltype(auto) move_element() {
		using IndicesOfSameAsT = typename types_type::template indices_of_types_same_as<T>;
		static_assert(IndicesOfSameAsT::size == 1);
		return move_element_at<IndicesOfSameAsT::template value_at_index<0>>();
	}

	template<typename F>
	auto& get(F&& f) {
		using Indices = typename types_type::template indices_of_types_that_are_args_for_invocable_type<F>;
		[&]<std::size_t... Indices0>(indices::of<Indices0...>) {
			( f(get_element_at<Indices0>()), ... );
		}(Indices{});
		return *this;
	}

	template<typename F>
	decltype(auto) consume(F&& f) {
		using Indices = typename types_type::template indices_of_types_invocable_with<F>;

		[&]<std::size_t... Indices0>(indices::of<Indices0...>) {
			( f(move_element_at<Indices0>()), ... );
		}(Indices{});

		return erase_elements_at(Indices{});
	}

	template<typename F>
	decltype(auto) consume_if_present(F&& f) {
		using Indices = typename types_type::template indices_of_invocable_with<F>;

		if constexpr(Indices::template size > 0) {
			return consume<F>(std::forward<F>(f));
		}
		else {
			return std::move(*this);
		}
	}

	template<typename T>
	using indices_of_elements_same_as = typename types_type::template indices_of_types_same_as<T>;

	template<typename T>
	auto move_elements_same_as() {
		return move_elements_at(indices_of_elements_same_as<T>{});
	}

	template<typename T>
	auto erase_elements_same_as() {
		return erase_elements_at(indices_of_elements_same_as<T>{});
	}

	template<std::size_t Index>
	decltype(auto) element_at_index() const {
		return std::get<Index>(*this);
	}

	template<std::size_t... Indices>
	auto elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ element_at<Indices>() ... };
	}

	template<std::size_t Index>
	auto erase_element_at() {
		return move_elements_at(typename indices_type::template erase_value_at_index<Index>{});
	}

	template<std::size_t... Indices>
	auto erase_elements_at(indices::of<Indices...> = {}) {
		using Indices0 = typename indices_type::template erase_values_at_indices<Indices...>;
		return move_elements_at(Indices0{});
	}

};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;