#pragma once

#include "../is.hpp"
#include <utility>

namespace u {

template<typename T, T... Values>
struct is<std::integer_sequence<T, Values...>> {

	template<T V>
	static constexpr bool contains = ((Values == V) || ... );
};

}