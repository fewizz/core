#pragma once

#include "./basic.hpp"

struct range_extensions_options {
	bool generate_size_from_iterator_and_sentinel_distance = false;
};

template<
	typename Derived,
	range_extensions_options Options = range_extensions_options{}
>
struct range_extensions;