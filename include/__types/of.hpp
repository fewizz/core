#pragma once

#include "../__values/of.hpp"

namespace __types {

	template<typename... Types> 
	struct of {
		static constexpr nuint size = sizeof...(Types);
		static constexpr bool is_empty = size == 0;

		using indices = ::indices::from<0>::to<size>;

		template<typename To>
		using pass_for_type = typename To::template for_types<Types...>;

		template<template<typename...> typename Type>
		using pass_for_type_directly = Type<Types...>;

		template<typename Transformer>
		using transform = __types::of<
			typename Transformer::template for_type<Types>...
		>;

	};

}