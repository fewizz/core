#pragma once

#include "../integer.hpp"

namespace types {

template<typename... Types> 
struct of {
	static constexpr uint size = sizeof...(Types);
	static constexpr bool is_empty = size == 0u;

	template<typename Func>
	using pass_for_type = typename Func::template for_types_of<Types...>;

	template<template<typename...> typename Type>
	using pass_for_type_directly = Type<Types...>;
};

}