#pragma once

#include "./basic.hpp"
#include "./iterator.hpp"
#include "./sentinel.hpp"
#include "../forward.hpp"
#include "../write.hpp"

namespace __range {

// hope it will use memcpy

template<basic_range Range>
class copy {
	Range from_;
public:

	constexpr copy(Range&& range) : from_{ forward<Range>(range) } {}

	template<basic_range To>
	constexpr void to(To&& to) {
		auto from_begin = range_iterator(from_);
		auto from_end   = range_sentinel(from_);
		auto to_begin   = range_iterator(to);

		while(from_begin != from_end) {
			*to_begin = *from_begin;
			++from_begin;
			++to_begin;
		}
	}

	template<typename To>
	constexpr auto to_output_stream(To&& to) {
		auto from_begin = range_iterator(from_);
		auto from_end   = range_sentinel(from_);

		while(from_begin != from_end) {
			::write(*from_begin, to);
			++from_begin;
		}
	}

};

template<basic_range Range>
copy(Range&&) -> copy<Range>;

} // __range