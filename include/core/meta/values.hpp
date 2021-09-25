#pragma once

#include "../integer.hpp"

template<auto... Values>
struct values_of;

template<uint... Values>
using indices_of = values_of<Values...>;

template<uint IndexFrom>
class indices_from {
	
	template<uint IndexTo>
	struct to_t {
		static constexpr uint size = IndexTo - IndexFrom;

		template<uint... Indices>
		struct result;

		template<uint... Indices>
		requires(sizeof...(Indices) == size)
		struct result<Indices...> {
			using type = indices_of<Indices...>;
		};

		template<uint... Indices>
		requires(sizeof...(Indices) == 0 && size > 0u)
		struct result<Indices...> {
			using type = typename result<IndexFrom>::type;
		};

		template<uint... Indices>
		requires(sizeof...(Indices) > 0 && sizeof...(Indices) != size)
		struct result<Indices...> {
			using type = typename result<Indices..., IndexFrom + sizeof...(Indices)>::type;
		};
	};
public:

	template<uint IndexTo>
	using to = typename to_t<IndexTo>::template result<>::type;
}; // from

template<auto... Values>
struct values_of {
	static constexpr uint size = sizeof...(Values);
	static constexpr bool empty = size == 0u;

	using indices = indices_from<0u>::to<size>;

	template<typename T>
	static constexpr auto pass_for_value = T::template for_values_of<Values...>;
};