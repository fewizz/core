#pragma once

#include <iterator>
#include <concepts>
#include <optional>
#include <type_traits>

namespace u {

namespace internal {
	template<class T>
	concept has_iterator_concept = requires {
		typename T::iterator_concept;
	};

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
	/*template<class It>
	struct iterator_has_iterator_concept : std::false_type {};

	template<has_iterator_concept It>
	struct iterator_has_iterator_concept<It> : std::true_type {};

	template<class It>
	struct iterator_traits_has_iterator_concept : std::false_type {};
	
	template<class It>
	requires requires {
		typename std::iterator_traits<It>::concept_type;
	}
	struct iterator_traits_has_iterator_concept<It> : std::true_type {};

	template<
		class It,
		bool = iterator_has_iterator_concept<It>::value,
		bool = iterator_traits_has_iterator_concept<It>::value
	>
	struct iterator_concept {
		using type = typename std::iterator_traits<It>::iterator_category;
	};

	template<class It, bool b>
	struct iterator_concept<It, true, b> {
		using type = typename It::iterator_category;
	};

	template<class It>
	struct iterator_concept<It, false, true> {
		using type = typename std::iterator_traits<It>::iterator_concept;
	};*/
}

template<class It>
using iter_concept_t = typename internal::iterator_concept<It>::type;


template<class It>
concept iterator_of_bytes
	= std::input_or_output_iterator<It>
	&& std::same_as<std::byte, std::iter_value_t<It>>;

enum class advance_direction : int {
	forward = 1, backward = -1
};

template<advance_direction AD, class Iterator, class Sentinel, class Distance>
std::optional<Iterator>
advance(Iterator it, Sentinel s, Distance d) {
	if constexpr(std::random_access_iterator<Iterator>) {
		if constexpr(AD == advance_direction::forward) {
			it += d;
			if(it > s) return {};
		}
		else {
			it -= d;
			if(it < s) return {};
		}
		
		return { it };
	}
	else {
		while(d != 0) {
			if(it == s) return { };

			if constexpr(AD == advance_direction::forward) {
				++it;
				--d;
			}
			else {
				--it;
				++d;
			}
		}

		return { it };
	}
}

template<class Iterator, class Sentinel, class Distance>
std::optional<Iterator>
advance_forward(Iterator it, Sentinel s, Distance d) {
	return advance<advance_direction::forward>(it, s, d);
}

template<class Iterator, class Sentinel, class Distance>
std::optional<Iterator>
advance_backward(Iterator it, Sentinel s, Distance d) {
	return advance<advance_direction::backward>(it, s, d);
}

template<class Iterator, class Sentinel, class Distance>
std::optional<Iterator>
advance(Iterator it, Sentinel s_begin, Sentinel s_end, Distance d) {
	if(d >= 0) {
		return advance_forward(it, s_end, d);
	}
	else {
		return advance_backward(it, s_begin, -d);
	}
}



}