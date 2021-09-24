#pragma once

#include "../integer.hpp"
#include "../forward.hpp"
#include "../move.hpp"
#include "../meta/indices.hpp"
#include "../meta/types.hpp"
#include "../meta/first_type.hpp"

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

	template<uint Index>
	auto& at() const & {
		if constexpr(Index == 0u) return value;
		else {
			static_assert(Index > 0u);
			return tuple_base<Tail...>::template get<Index - 1u>();
		}
	}

	template<uint Index>
	auto&& at() const && {
		if constexpr(Index == 0u) return move(value);
		else {
			static_assert(Index > 0u);
			return move(
				tuple_base<Tail...>::template get<Index - 1u>()
			);
		}
	}
};

template<typename... Types>
struct tuple : private tuple_base<Types...> {
	static constexpr uint size = sizeof...(Types);
	using indices = typename indices_from<0u>::to<size>;
	using types = types_of<Types...>;

	tuple(Types&&... values)
		: tuple_base<Types...>{ forward<Types>(values)... }
	{}

	template<uint Index>
	decltype(auto) at() const {
		return tuple_base<Types...>::template at<Index>();
	}

	template<uint... Indices>
	auto pass_to(auto&& func, indices_of<Indices...> = indices{}) {
		return func(at<Indices>() ... );
	}
};

template<typename... Ts>
tuple(Ts&&... ts) -> tuple<Ts...>;