#pragma once

#include "../types/at_index.hpp"
#include "../forward.hpp"
#include "../integer.hpp"
#include "../move.hpp"
#include "../types/of.hpp"
#include "../values/of.hpp"

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
		constexpr auto& at() { return element; }

		template<nuint Index> requires(Index > 0)
		constexpr auto& at() { return next::template at<Index - 1>(); }

		template<nuint Index> requires(Index == 0)
		constexpr auto& at() const { return element; }

		template<nuint Index> requires(Index > 0)
		constexpr auto& at() const { return next::template at<Index - 1>(); }
	};

	template<typename... Types>
	struct of {
		static constexpr nuint size = sizeof...(Types);
		using indices = typename indices::from<0>::to<size>;
		using types = types::of<Types...>;

	private:
		elements::recursive_elements_storage<Types...> m_storage;

	public:
	
		constexpr of(Types&&... values)
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
			[&]<nuint... Indices>(::indices::of<Indices...>) {
				(f(at<Indices>()) , ...);
			}(indices{});
		}
		
		template<typename F>
		void for_each(F&& f) {
			[&]<nuint... Indices>(::indices::of<Indices...>) {
				(f(at<Indices>()) , ...);
			}(indices{});
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

}