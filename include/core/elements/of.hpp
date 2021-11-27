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

	template<uint Index>
	constexpr auto& at() const {
		if constexpr(Index == 0) return element;
		else {
			static_assert(Index > 0);
			return next::template at<Index - 1>();
		}
	}
};

namespace elements {

	template<typename... Types>
	struct of {
		static constexpr uint size = sizeof...(Types);
		using indices = typename indices::from<0>::to<size>;
		using types = types::of<Types...>;

	private:
		recursive_elements_storage<Types...> m_storage;

	public:
	
		constexpr of(Types&&... values)
			: m_storage {
				forward<Types>(values)...
			}
		{}
	
		template<uint Index>
		constexpr auto& refer_at_index() const {
			return m_storage.template at<Index>();
		}

		template<uint Index>
		constexpr auto&& move_at_index() const {
			return move(m_storage.template at<Index>());
		}

		
	
		/*template<uint... Indices>
		constexpr auto pass_to(auto&& func, ::indices::of<Indices...> = indices{}) {
			return func(at_index<Indices>() ... );
		}

		template<typename... OtherTypes, uint... Indices>
		constexpr auto append_back(OtherTypes&&... values, ::indices::of<Indices...> = indices{}) const {
			return elements::of<Types..., OtherTypes...>{
				move(at_index<Indices>())...,
				forward<OtherTypes>(values)...
			};
		}*/
	};
	
	template<typename... Types>
	of(Types&&... ts) -> of<Types...>;

}