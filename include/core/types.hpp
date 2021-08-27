#pragma once

#include "is.hpp"
#include "are.hpp"
#include "at.hpp"
#include "indices_of_types_that_satisfy.hpp"

namespace types {

template<typename... Ts> 
struct of;

template<>
struct of<> {
	static constexpr std::size_t size = 0;
	static constexpr bool empty = true;

	template<class T>
	static constexpr bool contains = false;

	template<class T>
	using append_back = of<T>;

	template<class T>
	using append_front = of<T>;

	template<typename T>
	static constexpr std::size_t count = 0;

	template<typename T>
	using indices_of_same_as = indices::of<>;

	template<typename T>
	using indices_of_not_same_as = indices::of<>;

	template<typename... Args>
	using indices_of_invocable_with = indices::of<>;

	template<typename... Args>
	using indices_of_not_invocable_with = indices::of<>;
};

template<typename... Ts>
struct of {
	static constexpr std::size_t size = sizeof...(Ts);
	static constexpr bool empty = false;

	template<std::size_t Index>
	using type_at = typename type::at<Index>::template of_types<Ts...>;

	template<std::size_t... Indices>
	using types_at = of<type_at<Indices>...>;

	using front = type_at<0>;
	using back = type_at<size - 1>;

	template<typename... Ts0>
	using append_back = of<Ts..., Ts0...>;

	template<typename... Ts0>
	using append_front = of<Ts0..., Ts...>;

	template<typename T>
	using indices_of_same_as =
		typename indices_of_types_that_satisfy<
			is::type<T>::template same_as_predicate
		>::template for_types<Ts...>;
	
	template<typename T>
	using indices_of_not_same_as =
		typename indices_of_types_that_not_satisfy<
			is::type<T>::template same_as_predicate
		>::template for_types<Ts...>;

	template<typename T>
	static constexpr bool contains = indices_of_same_as<T>::size() != 0;

	template<typename... Args>
	using indices_of_invocable_with =
		typename indices_of_types_that_satisfy<
			are::types<Args...>::template args_for_predicate
		>::template for_types<Ts...>;

	template<typename... Args>
	using indices_of_not_invocable_with =
		typename indices_of_types_that_not_satisfy<
			are::types<Args...>::template args_for_predicate
		>::template for_types<Ts...>;
};

}