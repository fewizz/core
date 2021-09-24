#pragma once

#include "integer.hpp"

template<auto... Values>
struct values_of {
	uint size = sizeof...(Values);
};