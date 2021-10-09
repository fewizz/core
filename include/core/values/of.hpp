#pragma once

#include "../integer.hpp"

namespace values {

	template<auto... Values>
	struct of;
}

namespace indices {
	template<uint... Values>
	using of = values::of<Values...>;

	template<uint IndexFrom>
	class from {
	
	template<uint IndexTo>
	struct to_t {
		static constexpr uint size = IndexTo - IndexFrom;

		template<uint... Indices>
		struct result;

		template<uint... Indices>
		requires(size == sizeof...(Indices))
		struct result<Indices...> {
			using type = indices::of<Indices...>;
		};

		template<uint... Indices>
		requires(size != sizeof...(Indices) && sizeof...(Indices) == 0 && size > 0u)
		struct result<Indices...> {
			using type = typename result<IndexFrom>::type;
		};

		template<uint... Indices>
		requires(size != sizeof...(Indices) && sizeof...(Indices) > 0)
		struct result<Indices...> {
			using type = typename result<Indices..., IndexFrom + sizeof...(Indices)>::type;
		};
	};
public:

	template<uint IndexTo>
	using to = typename to_t<IndexTo>::template result<>::type;
}; // from

}

namespace values {
	
	template<auto... Values>
	struct of {
		static constexpr uint size = sizeof...(Values);
		static constexpr bool is_empty = size == 0u;

		using indices = indices::from<0u>::to<size>;

		template<typename T>
		static constexpr auto pass_for_value = T::template for_values_of<Values...>;

		template<typename T>
		using pass_for_type = typename T::template for_values_of<Values...>;

		template<template<auto...> typename Type>
		using pass_for_type_directly = Type<Values...>;

	};
}