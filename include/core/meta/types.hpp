#pragma once

#include <cstddef>

namespace types {

template<typename... Types> 
struct of {
	std::size_t size = sizeof...(Types);
};

}