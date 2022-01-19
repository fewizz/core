#pragma once

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

		recursive_elements_storage(HeadType&& head_element, TailTypes&&... tail_elements)
		:
			next {
				forward<TailTypes>(tail_elements)...
			},
			element {
				forward<HeadType>(head_element)
			}
		{}

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
				forward<Types>(values)...
			}
		{}
	
		template<nuint Index>
		constexpr auto& at() const {
			return m_storage.template at<Index>();
		}

		template<nuint Index>
		constexpr auto& at() {
			return m_storage.template at<Index>();
		}

		template<nuint... Indices>
		void for_each(auto&& f, values::of<Indices...> = indices{}) const {
			(f(at<Indices>()) , ...);
		}
	};
	
	template<typename... Types>
	of(Types&&... ts) -> of<Types...>;

}