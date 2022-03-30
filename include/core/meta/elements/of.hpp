#pragma once

#include "../types/of.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"
#include "../values/of.hpp"
#include "../../forward.hpp"
#include "../../move.hpp"

namespace elements {

	template<nuint Index, typename Type>
	struct element_storage {
		Type element;

		constexpr element_storage(Type&& element) :
			element{ forward<Type>(element) }
		{}

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

	private:

		template<nuint Index, typename Type>
		constexpr auto& get_storage(element_storage<Index, Type>* ptr) {
			return ptr->element;
		}

		template<nuint Index, typename Type>
		constexpr auto& get_storage(element_storage<Index, Type>* ptr) const {
			return ptr->element;
		}

	public:
	
		constexpr of(Types&&... values) : 
			element_storage<Indices, Types>(::forward<Types>(values))...
		{}

		constexpr of(of&&) = default;
	
		template<nuint Index>
		constexpr decltype(auto) at() const {
			return get_storage<Index>(this);
		}

		template<nuint Index>
		constexpr decltype(auto) at() {
			return get_storage<Index>(this);
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
		void for_each(F&& f) const {
			(f(at<Indices>()) , ...);
		}
		
		template<typename F>
		void for_each(F&& f) {
			(f(at<Indices>()) , ...);
		}

		template<typename F, nuint... OtherIndices>
		void for_each(F&& f, ::indices::of<OtherIndices...>) const {
			(f(at<OtherIndices>()) , ...);
		}
		
		template<typename F, nuint... OtherIndices>
		void for_each(F&& f, ::indices::of<OtherIndices...>) {
			(f(at<OtherIndices>()) , ...);
		}

		decltype(auto) pass(auto&& f) const {
			return f(at<Indices>()...);
		}

		decltype(auto) pass(auto&& f) {
			return f(at<Indices>()...);
		}

		decltype(auto) forward(auto&& f) const {
			return f(::forward<Types>(at<Indices>())...);
		}

		decltype(auto) forward(auto&& f) {
			return f(::forward<Types>(at<Indices>())...);
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

#include "core/std/tuple_size.hpp"

template<typename... Types>
struct std::tuple_size<elements::of<Types...>> {
	static constexpr nuint value = sizeof...(Types);
};

#include "core/std/tuple_element.hpp"
#include "core/meta/types/at_index.hpp"

template<nuint Index, typename... Types>
struct std::tuple_element<Index, elements::of<Types...>> {
	using type = typename types::at_index<Index>::template for_types<Types...>;
};