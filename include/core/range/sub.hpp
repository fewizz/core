#pragma once

#include "value_type.hpp"

namespace range {

	template<typename BeginIt, typename Sentinel>
	struct sub {
		BeginIt it;
		Sentinel sentinel;

		sub(BeginIt begin, Sentinel sentinel) :
			it{ begin },
			sentinel{ sentinel }
		{}

		using value_type = decay<decltype(*it)>;

		BeginIt begin() const { return it; }
		Sentinel end() const { return sentinel; }

		constexpr decltype(auto) operator [] (nuint index) const {
			return it[index];
		}

		constexpr decltype(auto) operator [] (nuint index) {
			return it[index];
		}

		// TODO if contiguous
		constexpr nuint size() const { return sentinel - it; }
	};

}; // range