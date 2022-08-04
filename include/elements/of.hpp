#pragma once

#include "../types/of.hpp"
#include "../types/at_index.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"
#include "../values/of.hpp"
#include "../forward.hpp"
#include "../move.hpp"

namespace elements {

	template<nuint Index, typename Type>
	class element_storage {
		Type element_;
	public:

		constexpr element_storage(Type&& element) :
			element_{ ::forward<Type>(element) }
		{}

		constexpr auto& get() const {
			return element_;
		}

		constexpr auto& get() {
			return element_;
		}

		constexpr decltype(auto) forward() const {
			return ::forward<Type>(element_);
		}

		constexpr decltype(auto) forward() {
			return ::forward<Type>(element_);
		}

	};

	// Inspired by https://youtu.be/TyiiNVA1syk
	template<typename... Types>
	struct of;

	template<typename... Types>
	struct of : of<indices::from<0>::to<sizeof...(Types)>, Types...> {
		using base_type = of<indices::from<0>::to<sizeof...(Types)>, Types...>;
		//using base_type::base_type; // TODO crashes clangd

		constexpr of(Types... elements) :
			base_type(forward<Types>(elements)...)
		{}

	};

	template<nuint... Indices, typename... Types>
	struct of<indices::of<Indices...>, Types...> :
		element_storage<Indices, Types>...
	{
		static constexpr nuint size = sizeof...(Types);
		using indices = indices::from<0>::to<size>;
		using types = types::of<Types...>;

		template<typename Type>
		static constexpr bool only_one_such_type =
			::types::count_of_satisfying_predicate<
				type::is_same_as<Type>
			>::template
			for_types<Types...> == 1;

		template<typename Type>
		static constexpr nuint type_index =
			::types::index_of_satisfying_predicate<
				type::is_same_as<Type>
			>::template
			for_types<Types...>;

		template<nuint Index>
		using type_at =
			typename ::types::template
			at_index<Index>::template
			for_types<Types...>;

	private:

		template<nuint Index, typename Type>
		constexpr auto&
		get_from_storage(const element_storage<Index, Type>* ptr) const {
			return ptr->get();
		}

		template<nuint Index, typename Type>
		constexpr auto&
		get_from_storage(element_storage<Index, Type>* ptr) {
			return ptr->get();
		}

		template<nuint Index, typename Type>
		constexpr decltype(auto)
		forward_from_storage(const element_storage<Index, Type>* ptr) const {
			return ptr->forward();
		}

		template<nuint Index, typename Type>
		constexpr decltype(auto)
		forward_from_storage(element_storage<Index, Type>* ptr) {
			return ptr->forward();
		}
	public:
	
		constexpr of(Types... values) :
			element_storage<Indices, Types>(::forward<Types>(values))...
		{}
	
		template<nuint Index>
		constexpr decltype(auto) at() const {
			return get_from_storage<Index>(this);
		}

		template<nuint Index>
		constexpr decltype(auto) at() {
			return get_from_storage<Index>(this);
		}

		template<typename Type>
		requires only_one_such_type<Type>
		constexpr const Type& get() const {
			return at<type_index<Type>>();
		}

		template<typename Type>
		requires only_one_such_type<Type>
		constexpr Type& get() {
			return at<type_index<Type>>();
		}

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
	of(Types&&... ts) -> of<Types...>;

	template<nuint Index, typename... Types>
	constexpr auto&& get(elements::of<Types...>&& elems) {
		return elems.template at<Index>();
	}

	template<nuint Index, typename... Types>
	constexpr auto&& get(const elements::of<Types...>&& elems) {
		return elems.template at<Index>();
	}

	template<nuint Index, typename... Types>
	constexpr auto& get(elements::of<Types...>& elems) {
		return elems.template at<Index>();
	}

	template<nuint Index, typename... Types>
	constexpr auto&& get(const elements::of<Types...>& elems) {
		return elems.template at<Index>();
	}

} // elements

template<typename... Types0, typename... Types1>
constexpr inline bool operator == (
	const elements::of<Types0...>& e0,
	const elements::of<Types1...>& e1
) {
	return e0.pass([&](auto&... e0) {
		return e1.pass([&](auto&... e1) {
			return ((e1 == e0) && ...);
		});
	});
}

#include "std/tuple_size.hpp"

template<typename... Types>
struct std::tuple_size<elements::of<Types...>> {
	static constexpr nuint value = sizeof...(Types);
};

#include "std/tuple_element.hpp"

template<nuint Index, typename... Types>
struct std::tuple_element<Index, elements::of<Types...>> {
	using type = typename types::at_index<Index>::template for_types<Types...>;
};