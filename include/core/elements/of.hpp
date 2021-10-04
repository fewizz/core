#pragma once

#include "../integer.hpp"
#include "../forward.hpp"
#include "../move.hpp"
#include "../types/of.hpp"
#include "../values/of.hpp"

template<typename... Types>
struct recursive_elements_storage{};

template<typename HeadType, typename... TailTypes>
struct recursive_elements_storage<HeadType, TailTypes...> : recursive_elements_storage<TailTypes...> {
	HeadType element;

	recursive_elements_storage(HeadType&& head_element, TailTypes&&... tail_elements)
	:
		element {
			forward<HeadType>(head_element)
		},
		recursive_elements_storage<TailTypes...> {
			forward<TailTypes>(tail_elements)...
		}
	{}

	template<uint Index>
	constexpr auto& at() const & {
		if constexpr(Index == 0u) return element;
		else {
			static_assert(Index > 0u);
			return recursive_elements_storage<TailTypes...>::template get<Index - 1u>();
		}
	}

	template<uint Index>
	constexpr auto&& at() const && {
		if constexpr(Index == 0u) return move(element);
		else {
			static_assert(Index > 0u);
			return move(
				recursive_elements_storage<TailTypes...>::template get<Index - 1u>()
			);
		}
	}
};

namespace elements {

	template<typename... Types>
	struct of : private recursive_elements_storage<Types...> {
		static constexpr uint size = sizeof...(Types);
		using indices = typename indices_from<0u>::to<size>;
		using types = types::of<Types...>;
	
		constexpr of(Types&&... values)
			: recursive_elements_storage<Types...>{
				forward<Types>(values)...
			}
		{}
	
		template<uint Index>
		constexpr decltype(auto) at() const {
			return recursive_elements_storage<Types...>::template at<Index>();
		}
	
		template<uint... Indices>
		constexpr auto pass_to(auto&& func, indices_of<Indices...> = indices{}) {
			return func(at<Indices>() ... );
		}
	};
	
	template<typename... Types>
	of(Types&&... ts) -> of<Types...>;

}