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
}

template<class It>
using iter_concept_t = typename internal::iterator_concept<It>::type;

namespace c {

template<class It>
concept iterator_of_bytes =
	std::same_as<
		std::iter_value_t<std::remove_cvref_t<It>>,
		std::byte
	>;
}

enum class advance_direction : signed char {
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