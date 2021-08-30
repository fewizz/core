#pragma once

#include <tuple>
#include "at.hpp"
#include "indices_of.hpp"
#include "is.hpp"
#include "types.hpp"
#include "values.hpp"

/*
size_t     size_value
size_t     size()

bool       contains()

size_t     count()

element    move_element_at<Index>()
tuple      move_elements_at<Indices...>(indices::of<Indices...>)

element    get_element_at<Index>()
tuple      get_elements_at<Indices...>(indices::of<Indices...>)

tuple      move_elements_of_type<Type>()
tuple      get_elements_of_type<Type>()

tuple&     for_each<F>(F&&)
*/

template<typename... Ts>
struct tuple : std::tuple<Ts...> {
	static constexpr std::size_t size_value = sizeof...(Ts);
	using indices_type = typename indices::from<0>::to<size_value>;
	using types_type = types::of<Ts...>;

	constexpr std::size_t size() const {
		return size_value;
	}

	template<typename T>
	constexpr bool contains() const {
		return types_type::template contains_type<T>;
	}

	template<typename T>
	constexpr std::size_t count() {
		return types_type::template count_of_type<T>;
	}

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
	auto move_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ move_element_at<Indices>() ... };
	}

	template<std::size_t Index>
	decltype(auto) get_element_at() {
		return std::get<Index>(*this);
	}

	template<std::size_t... Indices>
	auto get_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ get_element_at<Indices>() ... };
	}

	template<typename T>
	auto move_elements_of_type() {
		return move_elements_at<>(
			typename types_type::template indices_of_types_same_as<T>{}
		);
	}

	template<typename T>
	auto get_elements_of_type() {
		return get_elements_at<
			typename types_type::template indices_of_types_same_as<T>{}
		>();
	}

	template<typename F>
	auto& for_each(F&& f) {
		[&]<std::size_t... Indices>(indices::of<Indices...>) {
			( f(get_element_at<Indices>()), ... );
		}(indices_type{});
		return *this;
	}

	template<typename F>
	auto& get(F&& f) {
		return for_each([&]<typename T>(T& v) {
			if constexpr(is::type<F>::template invocable_with_arg<T&>) {
				f(v);
			}
		});
	}

	template<typename F>
	auto& move(F&& f) {
		return for_each([&]<typename T>(T& v) {
			if constexpr(is::type<F>::template invocable_with_arg<T&&>) {
				f(std::move(v));
			}
		});
	}
};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;