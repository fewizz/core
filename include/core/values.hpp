#pragma once

#include <cstddef>

namespace values {

template<auto... Values>
struct of {
	static constexpr std::size_t size = sizeof...(Values);
};

}