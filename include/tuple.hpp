#pragma once

#include "./__types/at_index.hpp"
#include "./__types/index_of_satisfying_predicate.hpp"
#include "./__types/indices_of_satisfying_predicate.hpp"
#include "./__type/copy_const_ref.hpp"
#include "./__values/of.hpp"
#include "./forward.hpp"


// Inspired by https://youtu.be/TyiiNVA1syk
template<typename... Types>
struct tuple;

template<typename... Types>
struct tuple : tuple<indices::from<0>::to<sizeof...(Types)>, Types...> {
	using base_type = tuple<indices::from<0>::to<sizeof...(Types)>, Types...>;

	constexpr tuple() : base_type{} {}

	constexpr tuple(Types... elements) requires(sizeof...(Types) > 0) :
		base_type(forward<Types>(elements)...)
	{}

};

template<typename... Types>
tuple(Types&&... ts) -> tuple<Types...>;


namespace __tuple {
	template<nuint Index, typename Type>
	struct element_storage {
		Type element_;

		constexpr element_storage() {}
		constexpr element_storage(Type&& element) :
			element_{ ::forward<Type>(element) }
		{}
	};
}


template<nuint... Indices, typename... Types>
struct tuple<indices::of<Indices...>, Types...> :
	private __tuple::element_storage<Indices, Types>...
{
	static constexpr nuint size = sizeof...(Types);
	using indices = indices::from<0>::to<size>;
	using types = __types::of<Types...>;

	template<type_predicate auto TypePredicate>
	static constexpr nuint type_index_satisfying_predicate =
		__types::index_of_satisfying_predicate<
			TypePredicate
		>::template for_types<Types...>;

	template<nuint Index>
	using type_at = type_at_index<Index, Types...>;

	constexpr tuple() : __tuple::element_storage<Indices, Types>()...{}
	constexpr tuple(Types... values) requires(sizeof...(Types) > 0) :
		__tuple::element_storage<Indices, Types>(::forward<Types>(values))...
	{}

	template<nuint Index, typename Self>
	constexpr decltype(auto) get(this Self&& self) {
		return (::forward<
			copy_const_ref<
				Self,
				__tuple::element_storage<Index, type_at<Index>>
			>
		>(self).element_);
	}

	template<type_predicate auto TypePredicate, typename Self>
	constexpr decltype(auto) get(this Self&& self) {
		return ::forward<Self>(self).template
			get<type_index_satisfying_predicate<TypePredicate>>();
	}

	template<type_predicate auto TypePredicate, typename Self, typename Handler>
	constexpr decltype(auto) get_or(this Self&& self, Handler&& handler) {
		if constexpr ((TypePredicate == 1).template for_types<Types...>()) {
			return ::forward<Self>(self).template get<TypePredicate>();
		}
		else {
			return handler();
		}
	}

	template<typename Self, typename Handler>
	constexpr void for_each(
		this Self&& self,
		Handler&& handler
	) {
		(handler(::forward<Self>(self).template get<Indices>()) , ...);
	}

	template<typename Self, typename Handler, nuint... OtherIndices>
	constexpr void for_each(
		this Self&& self,
		Handler&& handler,
		::indices::of<OtherIndices...>
	){
		(handler(::forward<Self>(self).template get<OtherIndices>()) , ...);
	}

	template<typename Self, typename Handler>
	constexpr decltype(auto) pass(
		this Self&& self,
		Handler&& handler
	) {
		return handler(::forward<Self>(self).template get<Indices>()...);
	}

	template<typename Self, typename Handler, nuint... OtherIndices>
	constexpr decltype(auto)
	pass(
		this Self&& self,
		Handler&& handler,
		::indices::of<OtherIndices...>
	) {
		return handler(::forward<Self>(self).template get<OtherIndices>()...);
	}

	template<type_predicate auto TypePredicate, typename Self, typename Handler>
	constexpr decltype(auto)
	pass(
		this Self&& self,
		Handler&& handler
	) {
		return ::forward<Self>(self).pass(
			::forward<Handler>(handler),
			typename __types::indices_of_satisfying_predicate<
				TypePredicate
			>::template for_types<Types...>{}
		);
	}

	friend constexpr inline bool operator == (
		const tuple<Types...>& a,
		const tuple<Types...>& b
	) {
		return a.pass([&](auto&... a_elements) {
			return b.pass([&](auto&... b_elements) {
				return ((b_elements == a_elements) && ...);
			});
		});
	}

};

#include "std/tuple_size.hpp"  // IWYU pragma: keep

template<typename... Types>
struct std::tuple_size<tuple<Types...>> {
	static constexpr nuint value = sizeof...(Types);
};

#include "std/tuple_element.hpp"  // IWYU pragma: keep

template<nuint Index, typename... Types>
struct std::tuple_element<Index, tuple<Types...>> {
	using type = type_at_index<Index, Types...>;
};