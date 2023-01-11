#pragma once

#include "./__types/at_index.hpp"
#include "./__types/count_of_satisfying_predicate.hpp"
#include "./__types/index_of_satisfying_predicate.hpp"
#include "./__type/is_range_of.hpp"
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
		(count_of_satisfying_predicate<is_same_as<Type>> == 1)
		.template for_types<Types...>();

	template<auto TypePredicate>
	static constexpr nuint type_index_satisfying_predicate =
		__types::index_of_satisfying_predicate<
			TypePredicate
		>::template for_types<Types...>;

	template<nuint Index>
	using type_at = type_at_index<Index, Types...>;

	template<auto TypePredicate>
	using type_satisfying_predicate = type_at<
		type_index_satisfying_predicate<TypePredicate>
	>;

private:

	template<nuint Index, typename Type>
	constexpr const Type&  get_from_storage(
		const __tuple::element_storage<Index, Type>* ptr
	) const & {
		return ptr->get0();
	}
	template<nuint Index, typename Type>
	constexpr       Type&  get_from_storage(
		      __tuple::element_storage<Index, Type>* ptr
	)      & {
		return ptr->get0();
	}
	template<nuint Index, typename Type>
	constexpr const Type&& get_from_storage(
		const __tuple::element_storage<Index, Type>* ptr
	) const && {
		return ptr->forward();
	}
	template<nuint Index, typename Type>
	constexpr       Type&& get_from_storage(
		      __tuple::element_storage<Index, Type>* ptr
	)      && {
		return ptr->forward();
	}

	template<nuint Index, typename Type>
	constexpr const Type&& forward_from_storage(
		const __tuple::element_storage<Index, Type>* ptr
	) const {
		return ptr->forward();
	}
	template<nuint Index, typename Type>
	constexpr       Type&& forward_from_storage(
		      __tuple::element_storage<Index, Type>* ptr
	) {
		return ptr->forward();
	}
public:

	constexpr tuple(Types... values) :
		__tuple::element_storage<Indices, Types>(::forward<Types>(values))...
	{}

	template<nuint Index>
	constexpr const type_at<Index>&  get_at() const &  {
		return get_from_storage<Index>(this);
	}
	template<nuint Index>
	constexpr       type_at<Index>&  get_at()       &  {
		return get_from_storage<Index>(this);
	}
	template<nuint Index>
	constexpr const type_at<Index>&& get_at() const && {
		return move(*this).template get_from_storage<Index>(this);
	}
	template<nuint Index>
	constexpr       type_at<Index>&& get_at()       && {
		return move(*this).template get_from_storage<Index>(this);
	}

	template<auto TypePredicate>
	constexpr const type_satisfying_predicate<TypePredicate>&
	get_satisfying_predicate() const &  {
		return get_at<type_index_satisfying_predicate<TypePredicate>>();
	}
	template<auto TypePredicate>
	constexpr       type_satisfying_predicate<TypePredicate>&
	get_satisfying_predicate()       &  {
		return get_at<type_index_satisfying_predicate<TypePredicate>>();
	}

	template<auto TypePredicate>
	constexpr const type_satisfying_predicate<TypePredicate>&&
	get_satisfying_predicate() const && {
		return move(*this).template
			get_at<type_index_satisfying_predicate<TypePredicate>>();
	}
	template<auto TypePredicate>
	constexpr       type_satisfying_predicate<TypePredicate>&&
	get_satisfying_predicate()       && {
		return move(*this).template
			get_at<type_index_satisfying_predicate<TypePredicate>>();
	}

	template<typename Type>
	requires only_one_such_type<Type>
	constexpr const type_satisfying_predicate<is_same_as<Type>>&
	get_same_as() const &  {
		return get_satisfying_predicate<is_same_as<Type>>();
	}
	template<typename Type>
	requires only_one_such_type<Type>
	constexpr       type_satisfying_predicate<is_same_as<Type>>&
	get_same_as()       &  {
		return get_satisfying_predicate<is_same_as<Type>>();
	}
	template<typename Type>
	requires only_one_such_type<Type>
	constexpr const type_satisfying_predicate<is_same_as<Type>>&&
	get_same_as() const {
		return move(*this).template
			get_satisfying_predicate<is_same_as<Type>>();
	}
	template<typename Type>
	requires only_one_such_type<Type>
	constexpr       type_satisfying_predicate<is_same_as<Type>>&&
	forward_same_as()       {
		return move(*this).template
			forward_satisfying_predicate<is_same_as<Type>>();
	}

	template<typename Type>
	constexpr const type_satisfying_predicate<is_same_as<Type>.while_decayed>&
	get_decayed_same_as() const &  {
		return get_satisfying_predicate<is_same_as<Type>.while_decayed>();
	}
	template<typename Type>
	constexpr       type_satisfying_predicate<is_same_as<Type>.while_decayed>&
	get_decayed_same_as()       &  {
		return get_satisfying_predicate<is_same_as<Type>.while_decayed>();
	}
	template<typename Type>
	constexpr const type_satisfying_predicate<is_same_as<Type>.while_decayed>&&
	get_decayed_same_as() const && {
		return move(*this).template
			get_satisfying_predicate<is_same_as<Type>.while_decayed>();
	}
	template<typename Type>
	constexpr       type_satisfying_predicate<is_same_as<Type>.while_decayed>&&
	forward_decayed_same_as()       {
		return move(*this).template
			get_satisfying_predicate<is_same_as<Type>.while_decayed>();
	}

	template<typename Type>
	constexpr const type_satisfying_predicate<is_range_of<Type>>&
	get_range_of() const &  {
		return get_satisfying_predicate<is_range_of<Type>>();
	}
	template<typename Type>
	constexpr const type_satisfying_predicate<is_range_of<Type>>&
	get_range_of()       &  {
		return get_satisfying_predicate<is_range_of<Type>>();
	}
	template<typename Type>
	constexpr const type_satisfying_predicate<is_range_of<Type>>&&
	get_range_of() const && {
		return move(*this).template
			get_satisfying_predicate<is_range_of<Type>>();
	}
	template<typename Type>
	constexpr const type_satisfying_predicate<is_range_of<Type>>&
	get_range_of()       && {
		return move(*this).template
			get_satisfying_predicate<is_range_of<Type>>();
	}

	template<typename F>
	constexpr void for_each(F&& f) const {
		(f(get_at<Indices>()) , ...);
	}
	template<typename F>
	constexpr void for_each(F&& f) {
		(f(get_at<Indices>()) , ...);
	}

	template<typename F, nuint... OtherIndices>
	constexpr void for_each(F&& f, ::indices::of<OtherIndices...>) const {
		(f(get_at<OtherIndices>()) , ...);
	}
	
	template<typename F, nuint... OtherIndices>
	constexpr void for_each(F&& f, ::indices::of<OtherIndices...>) {
		(f(get_at<OtherIndices>()) , ...);
	}

	constexpr decltype(auto) pass(auto&& f) const {
		return f(get_at<Indices>()...);
	}
	constexpr decltype(auto) pass(auto&& f) {
		return f(get_at<Indices>()...);
	}

	template<typename F, nuint... OtherIndices>
	constexpr decltype(auto) pass(F&& f, ::indices::of<OtherIndices...>) const {
		return f(get_at<OtherIndices>()...);
	}
	template<typename F, nuint... OtherIndices>
	constexpr decltype(auto) pass(F&& f, ::indices::of<OtherIndices...>)       {
		return f(get_at<OtherIndices>()...);
	}

	constexpr decltype(auto) forward(auto&& f) const {
		return f(forward_at<Indices>()...);
	}

	constexpr decltype(auto) forward(auto&& f) {
		return f(forward_at<Indices>()...);
	}
};

template<typename... Types>
tuple(Types&&... ts) -> tuple<Types...>;

template<nuint Index, typename... Types>
constexpr auto&& get(tuple<Types...>&& elems) {
	return elems.template get_at<Index>();
}

template<nuint Index, typename... Types>
constexpr auto&& get(const tuple<Types...>&& elems) {
	return elems.template get_at<Index>();
}

template<nuint Index, typename... Types>
constexpr auto& get(tuple<Types...>& elems) {
	return elems.template get_at<Index>();
}

template<nuint Index, typename... Types>
constexpr auto&& get(const tuple<Types...>& elems) {
	return elems.template get_at<Index>();
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