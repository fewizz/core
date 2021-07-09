#pragma once

#include <iterator>
#include <concepts>
#include <optional>
#include <type_traits>

namespace u {

namespace internal {
	template<class It>
	struct iterator_concept {
		using type = typename std::iterator_traits<It>::iterator_category;
	};

	template<class It>
	requires requires { typename It::iterator_concept; }
	struct iterator_concept<It> {
		using type = typename It::iterator_concept;
	};

	template<class It>
	requires requires { typename std::iterator_traits<It>::iterator_concept; }
	struct iterator_concept<It> {
		using type = typename std::iterator_traits<It>::iterator_concept;
	};
}

template<class It>
using iter_concept_t = typename internal::iterator_concept<It>::type;

}