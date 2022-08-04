#pragma once

#include "./basic.hpp"

namespace range {

	// hope it will use internal memcpy

	template<basic_range Range>
	class copy {
		Range from;
	public:

		template<basic_range Range0>
		constexpr copy(Range0&& range) : from{ range } {}

		template<basic_range To>
		constexpr decltype(auto) to(To&& to) {
			auto from_begin = begin(from);
			auto from_end = end(from);
			auto to_begin = begin(to);

			while(from_begin != from_end) {
				*to_begin = *from_begin;
				++from_begin;
				++to_begin;
			}

			return forward<To>(to);
		}

		template<typename To>
		constexpr auto to(To to) {
			auto from_begin = begin(from);
			auto from_end = end(from);

			while(from_begin != from_end) {
				*to = *from_begin;
				++from_begin;
				++to;
			}

			return to;
		}

	};

	template<basic_range Range>
	copy(Range&&) -> copy<Range>;

} // range