#pragma once

#include <type_traits>

namespace u {

template<template<typename T> typename P>
struct predicate {

	template<typename T>
	struct negate : std::bool_constant<! P<T>::value> {};
};

}