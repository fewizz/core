#pragma once

#include "./__types/at_index.hpp"
#include "./__types/count_of_satisfying_predicate.hpp"
#include "./__types/index_of_satisfying_predicate.hpp"
#include "./__type/is_same_as_predicate.hpp"
#include "./__values/of.hpp"
#include "./forward.hpp"
#include "./move.hpp"
#include "./__tuple/element_storage.hpp"

// Inspired by https://youtu.be/TyiiNVA1syk
template<typename... Types>
struct tuple;

template<typename... Types>
struct tuple : tuple<indices::from<0>::to<sizeof...(Types)>, Types...> {
	using base_type = tuple<indices::from<0>::to<sizeof...(Types)>, Types...>;
	//using base_type::base_type; // TODO crashes clangd

	constexpr tuple(Types... elements) :
		base_type(forward<Types>(elements)...)
	{}

};

template<nuint... Indices, typename... Types>
struct tuple<indices::of<Indices...>, Types...> :
	__tuple::element_storage<Indices, Types>...
{
	static constexpr nuint size = sizeof...(Types);
	using indices = indices::from<0>::to<size>;
	using types = __types::of<Types...>;

	template<typename Type>
	static constexpr bool only_one_such_type =
		(
			count_of_satisfying_predicate<
				is_same_as<Type>
			> == 1
		).template for_types<Types...>();

	template<typename Type>
	static constexpr nuint type_index =
		__types::index_of_satisfying_predicate<
			is_same_as<Type>
		>::template for_types<Types...>;

	template<nuint Index>
	using type_at = type_at_index<Index, Types...>;

private:

	template<nuint Index, typename Type>
	constexpr auto&
	get_from_storage(const __tuple::element_storage<Index, Type>* ptr) const {
		return ptr->get();
	}

	template<nuint Index, typename Type>
	constexpr auto&
	get_from_storage(__tuple::element_storage<Index, Type>* ptr) {
		return ptr->get();
	}

	template<nuint Index, typename Type>
	constexpr decltype(auto)
	forward_from_storage(
		const __tuple::element_storage<Index, Type>* ptr
	) const {
		return ptr->forward();
	}

	template<nuint Index, typename Type>
	constexpr decltype(auto)
	forward_from_storage(__tuple::element_storage<Index, Type>* ptr) {
		return ptr->forward();
	}
public:

	constexpr tuple(Types... values) :
		__tuple::element_storage<Indices, Types>(::forward<Types>(values))...
	{}

	template<nuint Index>
	constexpr decltype(auto) at() const {
		return get_from_storage<Index>(this);
	}
	template<nuint Index>
	constexpr decltype(auto) at()       {
		return get_from_storage<Index>(this);
	}

	template<typename Type>
	requires only_one_such_type<Type>
	constexpr const Type& get() const { return at<type_index<Type>>(); }

	template<typename Type>
	requires only_one_such_type<Type>
	constexpr       Type& get()       { return at<type_index<Type>>(); }

	template<typename F>
	constexpr void for_each(F&& f) const {
		(f(at<Indices>()) , ...);
	}
	
	template<typename F>
	constexpr void for_each(F&& f) {
		(f(at<Indices>()) , ...);
	}

	template<typename F, nuint... OtherIndices>
	constexpr void for_each(F&& f, ::indices::of<OtherIndices...>) const {
		(f(at<OtherIndices>()) , ...);
	}
	
	template<typename F, nuint... OtherIndices>
	constexpr void for_each(F&& f, ::indices::of<OtherIndices...>) {
		(f(at<OtherIndices>()) , ...);
	}

	constexpr decltype(auto) pass(auto&& f) const {
		return f(at<Indices>()...);
	}

	constexpr decltype(auto) pass(auto&& f) {
		return f(at<Indices>()...);
	}

	template<nuint Index>
	constexpr decltype(auto) forward() const {
		return forward_from_storage<Index>(this);
	}

	template<nuint Index>
	constexpr decltype(auto) forward() {
		return forward_from_storage<Index>(this);
	}

	constexpr decltype(auto) forward(auto&& f) const {
		return f(forward_from_storage<Indices>(this)...);
	}

	constexpr decltype(auto) forward(auto&& f) {
		return f(forward_from_storage<Indices>(this)...);
	}
};

template<typename... Types>
tuple(Types&&... ts) -> tuple<Types...>;

template<nuint Index, typename... Types>
constexpr auto&& get(tuple<Types...>&& elems) {
	return elems.template at<Index>();
}

template<nuint Index, typename... Types>
constexpr auto&& get(const tuple<Types...>&& elems) {
	return elems.template at<Index>();
}

template<nuint Index, typename... Types>
constexpr auto& get(tuple<Types...>& elems) {
	return elems.template at<Index>();
}

template<nuint Index, typename... Types>
constexpr auto&& get(const tuple<Types...>& elems) {
	return elems.template at<Index>();
}

template<typename... Types>
constexpr inline bool operator == (
const tuple<Types...>& e0,
const tuple<Types...>& e1
) {
	return e0.pass([&](auto&... e0) {
		return e1.pass([&](auto&... e1) {
			return ((e1 == e0) && ...);
		});
	});
}

#include "std/tuple_size.hpp"

template<typename... Types>
struct std::tuple_size<tuple<Types...>> {
	static constexpr nuint value = sizeof...(Types);
};

#include "std/tuple_element.hpp"

template<nuint Index, typename... Types>
struct std::tuple_element<Index, tuple<Types...>> {
	using type = type_at_index<Index, Types...>;
};