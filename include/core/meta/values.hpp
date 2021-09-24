#pragma once

#include "integer.hpp"

template<auto... Values>
struct values_of {
	static constexpr uint size = sizeof...(Values);
};