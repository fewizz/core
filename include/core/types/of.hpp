#pragma once

#include "../integer.hpp"

namespace types {

template<typename... Types> 
struct of {
	static constexpr uint size = sizeof...(Types);

	template<template<typename... Types0> typename Func>
	using pass_to = Func<Types...>;
};

}