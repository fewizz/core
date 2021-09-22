#pragma once

#include "integer.hpp"

namespace values {

template<auto... Values>
struct of {
	uint size = sizeof...(Values);
};

} // values