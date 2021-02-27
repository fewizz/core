#pragma once

#include <iterator>
#include <concepts>

namespace u {

template<class It>
concept iterator_of_bytes
	= std::input_or_output_iterator<It>
	&& std::same_as<std::byte, std::iter_value_t<It>>;

}