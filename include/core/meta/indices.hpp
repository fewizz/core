#pragma once

#include "integer.hpp"
#include "values.hpp"

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