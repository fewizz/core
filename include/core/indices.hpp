#pragma once

#include "values.hpp"

namespace indices {

template<std::size_t... Indices>
struct of {

	template<std::size_t... Indices0>
	using append_back = of<Indices..., Indices0...>;

	template<std::size_t... Indices0>
	using append_front = of<Indices0..., Indices...>;

}; // of

template<std::size_t IndexFrom>
class from {
	
	template<std::size_t Index>
	struct to_t;

	template<>
	struct to_t<IndexFrom> {
		using type = of<IndexFrom>;
	};

	template<std::size_t Index>
	struct to_t {
		using type = typename of<Index>::template append_front<to_t<Index - 1>::type>;
	};
public:

	template<std::size_t IndexTo>
	using to = typename to_t<IndexTo>::type;
}; // from

} // indices