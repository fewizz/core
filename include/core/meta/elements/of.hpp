#pragma once

#include "../types/of.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../type/is_same_as.hpp"
#include "../values/of.hpp"
#include "../../forward.hpp"
#include "../../move.hpp"

namespace elements {

	template<typename... Types>
	struct recursive_elements_storage{};

	template<typename HeadType, typename... TailTypes>
	struct recursive_elements_storage<HeadType, TailTypes...> : elements::recursive_elements_storage<TailTypes...> {
		using next = recursive_elements_storage<TailTypes...>;

		HeadType element;

		constexpr recursive_elements_storage(HeadType&& head_element, TailTypes&&... tail_elements)
		:
			next {
				forward<TailTypes>(tail_elements)...
			},
			element {
				forward<HeadType>(head_element)
			}
		{}

		constexpr recursive_elements_storage(recursive_elements_storage&&) = default;

		template<nuint Index> requires(Index == 0)
		constexpr HeadType& at() { return element; }

		template<nuint Index> requires(Index > 0)
		constexpr auto& at() { return next::template at<Index - 1>(); }

		template<nuint Index> requires(Index == 0)
		constexpr const HeadType& at() const { return element; }

		template<nuint Index> requires(Index > 0)
		constexpr const auto& at() const { return next::template at<Index - 1>(); }
	};

	template<typename... Types>
	struct of {
		static constexpr nuint size = sizeof...(Types);
		using indices = typename indices::from<0>::to<size>;
		using types = types::of<Types...>;

		template<typename Type>
		static constexpr bool only_one_such_type =
			::types::count_of_satisfying_predicate<type::is_same_as<Type>>::template for_types<Types...> == 1;

	private:
		elements::recursive_elements_storage<Types...> m_storage;

	public:

		template<typename Type>
		static constexpr nuint type_index = ::types::index_of_satisfying_predicate<type::is_same_as<Type>>::template for_types<Types...>;
	
		constexpr of(Types... values)
			: m_storage {
				::forward<Types>(values)...
			}
		{}

		constexpr of(of&&) = default;
	
		template<nuint Index>
		constexpr auto& at() const {
			return m_storage.template at<Index>();
		}

		template<nuint Index>
		constexpr auto& at() {
			return m_storage.template at<Index>();
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		constexpr const Type& get() const {
			return at<type_index<Type>>();
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		constexpr Type& get() {
			return at<type_index<Type>>();
		}

		template<typename F, nuint... Indices>
		void for_each(F&& f, ::indices::of<Indices...>) const {
			(f(at<Indices>()) , ...);
		}
		
		template<typename F, nuint... Indices>
		void for_each(F&& f, ::indices::of<Indices...>) {
			(f(at<Indices>()) , ...);
		}

		template<typename F>
		void for_each(F&& f) const {
			for_each(forward<F>(f), indices{});
		}
		
		template<typename F>
		void for_each(F&& f) {
			for_each(forward<F>(f), indices{});
		}

		decltype(auto) pass(auto&& f) const {
			return [&]<nuint... Indices>(::indices::of<Indices...>) {
				return f(at<Indices>()...);
			}(indices{});
		}

		decltype(auto) pass(auto&& f) {
			return [&]<nuint... Indices>(::indices::of<Indices...>) {
				return f(at<Indices>()...);
			}(indices{});
		}

		decltype(auto) forward(auto&& f) const {
			return [&]<nuint... Indices, typename... Types0>(::indices::of<Indices...>, ::types::of<Types0...>) {
				return f(::forward<Types0>(at<Indices>())...);
			}(indices{}, types{});
		}

		decltype(auto) forward(auto&& f) {
			return [&]<nuint... Indices, typename... Types0>(::indices::of<Indices...>, ::types::of<Types0...>) {
				return f(::forward<Types0>(at<Indices>())...);
			}(indices{}, types{});
		}
	};
	
	template<typename... Types>
	of(Types&&... ts) -> of<Types...>;

	template<nuint Index, typename... Types>
	constexpr auto get(elements::of<Types...> elems) {
		return elems.template at<Index>();
	}

}

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