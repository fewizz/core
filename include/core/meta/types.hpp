#pragma once

#include "integer.hpp"

template<typename... Types> 
struct types_of {
	static constexpr uint size = sizeof...(Types);

	template<template<typename... Types0> typename Func>
	using pass_to = Func<Types...>;

	//template<template<typename... Types0> typename Func>
	//static constexpr auto pass_for_value = Func<Types...>::value;
};