#pragma once

#include <iterator>
#include <concepts>
#include <optional>

namespace u {

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