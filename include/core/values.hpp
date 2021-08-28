#pragma once

#include <cstddef>
#include "at.hpp"
#include "is.hpp"

namespace values {

template<auto... Values>
struct of;

template<>
struct of<> {
	static constexpr std::size_t size = 0;

	// append back
	template<auto... Values>
	using append_back = of<Values...>;

	// append front
	template<auto... Values>
	using append_front = of<Values...>;
};

template<auto... Values>
struct of {
	static constexpr std::size_t size = sizeof...(Values);

	// append back
	template<auto... Values0>
	using append_back = of<Values..., Values0...>;

	// append front
	template<auto... Values0>
	using append_front = of<Values0..., Values...>;

	// at
	template<std::size_t Index>
	static constexpr auto at = ::at<Index>::template of_values<Values...>;

	// front
	static constexpr auto front = at<0>;

	// back
	static constexpr auto back = at<size - 1>;
};

} // values