#pragma once

#include "../integer.hpp"
#include "../move.hpp"
#include "../types/of.hpp"
#include "../values/of.hpp"
#include "../forward.hpp"

namespace internal {
	template<typename... Types>
	struct recursive_elements_storage{};

	template<typename HeadType, typename... TailTypes>
	struct recursive_elements_storage<HeadType, TailTypes...> : recursive_elements_storage<TailTypes...> {
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

		template<nuint Index>
		constexpr auto& at() const & {
			if constexpr(Index == 0) return element;
			else {
				static_assert(Index > 0);
				return next::template at<Index - 1>();
			}
		}

		template<nuint Index>
		constexpr auto&& at() const && {
			if constexpr(Index == 0) return element;
			else {
				static_assert(Index > 0);
				return next::template at<Index - 1>();
			}
		}

		template<nuint Index>
		constexpr auto& at() & {
			if constexpr(Index == 0) return element;
			else {
				static_assert(Index > 0);
				return next::template at<Index - 1>();
			}
		}

		template<nuint Index>
		constexpr auto&& at() && {
			if constexpr(Index == 0) return element;
			else {
				static_assert(Index > 0);
				return next::template at<Index - 1>();
			}
		}
	};
}

namespace elements {

	template<typename... Types>
	struct of {
		static constexpr nuint size = sizeof...(Types);
		using indices = typename indices::from<0>::to<size>;
		using types = types::of<Types...>;

	private:
		internal::recursive_elements_storage<Types...> m_storage;

	public:
	
		constexpr of(Types&&... values)
			: m_storage {
				forward<Types>(values)...
			}
		{}
	
		template<nuint Index>
		constexpr decltype(auto) at_index() const {
			return m_storage.template at<Index>();
		}

		template<nuint Index>
		constexpr decltype(auto) at_index() {
			return m_storage.template at<Index>();
		}
	};
	
	template<typename... Types>
	of(Types&&... ts) -> of<Types...>;

}