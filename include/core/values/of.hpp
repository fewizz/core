#pragma once

#include "../integer.hpp"

namespace values {

	template<auto... Values>
	struct of;
}

namespace indices {
	template<nuint... Values>
	using of = values::of<Values...>;

	template<nuint IndexFrom>
	class from {
	
	template<nuint IndexTo>
	struct to_t {
		static constexpr nuint size = IndexTo - IndexFrom;

		template<nuint... Indices>
		struct result;

		template<nuint... Indices>
		requires(size == sizeof...(Indices))
		struct result<Indices...> {
			using type = indices::of<Indices...>;
		};

		template<nuint... Indices>
		requires(size != sizeof...(Indices) && sizeof...(Indices) == 0 && size > 0u)
		struct result<Indices...> {
			using type = typename result<IndexFrom>::type;
		};

		template<nuint... Indices>
		requires(size != sizeof...(Indices) && sizeof...(Indices) > 0)
		struct result<Indices...> {
			using type = typename result<Indices..., IndexFrom + sizeof...(Indices)>::type;
		};
	};
public:

	template<nuint IndexTo>
	using to = typename to_t<IndexTo>::template result<>::type;
}; // from

}

namespace values {
	
	template<auto... Values>
	struct of {
		static constexpr nuint size = sizeof...(Values);
		static constexpr bool is_empty = size == 0;

		using indices = indices::from<0>::to<size>;

		template<typename T>
		static constexpr auto pass_for_value = T::template for_values_of<Values...>;

		template<typename T>
		using pass_for_type = typename T::template for_values_of<Values...>;

		template<template<auto...> typename Type>
		using pass_for_type_directly = Type<Values...>;

	};
}