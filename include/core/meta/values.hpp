#pragma once

#include <cstddef>

namespace values {

template<auto... Values>
struct of {
	std::size_t size = sizeof...(Values);
};

} // values