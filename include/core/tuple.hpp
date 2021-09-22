#pragma once

#include "integer.hpp"
#include "forward.hpp"
#include "move.hpp"
#include "primitive_integer.hpp"

template<typename... Types>
struct tuple_base;

template<>
struct tuple_base<> {};

template<typename Head, typename... Tail>
struct tuple_base<Head, Tail...> : tuple_base<Tail...> {
	Head value;

	tuple_base(Head&& h, Tail&&... tail)
		: value{ forward<Head>(h) }, tuple_base<Tail...>{ forward<Tail>(tail)... }
	{}

	template<uint_native Index>
	auto& get() const & {
		if constexpr(Index == 0lu) return value;
		else {
			static_assert(Index > 0);
			return tuple_base<Tail...>::template get<Index - 1>();
		}
	}

	template<primitive::uint_native Index>
	auto&& get() const && {
		if constexpr(Index == 0) return move(value);
		else {
			static_assert(Index > 0);
			return move(
				tuple_base<Tail...>::template get<Index - 1>()
			);
		}
	}
};

template<typename... Types>
struct tuple : private tuple_base<Types...> {
	static constexpr uint_native size = sizeof...(Types);
	using indices_type = typename indices::from<0>::to<size>;
	using types_type = types::of<Types...>;

	template<typename T>
	static constexpr bool contains = types_type::template contains_same_as_type<T>;

	template<primitive::uint_native Index>
	decltype(auto) get_element_at() const {
		return tuple_base<Types...>::template get<Index>();
	}

	template<primitive::uint_native... Indices>
	auto get_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ get_element_at<Indices>() ... };
	}

	template<typename T>
	constexpr primitive::uint_native count() {
		return types_type::template count_of_same_as_type<T>;
	}

	tuple(Types&&... values)
		: tuple_base<Types...>{ forward<Types>(values)... }
	{}

	template<primitive::uint_native Index>
	decltype(auto) move_element_at() {
		return get_element_at<Index>();
	}

	template<primitive::uint_native... Indices>
	auto move_elements_at(indices::of<Indices...> = {}) {
		return ::tuple{ move_element_at<Indices>() ... };
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
	}*/
};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;