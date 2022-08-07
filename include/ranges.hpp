#pragma once

#include "./__range/basic.hpp"
#include "./__ranges/are_equal.hpp"
#include "./__ranges/concat_view.hpp"
#include "./elements/of.hpp"

template<basic_range... Ranges>
struct ranges {
private:
	elements::of<Ranges...> ranges_;
public:

	ranges(Ranges&&... ranges) : ranges_{ forward<Ranges>(ranges)... } {}

	constexpr bool are_equal() const {
		return ranges_.pass([](basic_range auto&... ranges) {
			return __ranges::are_equal(ranges...);
		});
	}

	constexpr __ranges::concat_view<Ranges...> concat_view() const {
		return ranges_.pass([](basic_range auto&... ranges) {
			return __ranges::concat_view{ ranges... };
		});
	}

	constexpr __ranges::concat_view<Ranges...> concat_view() {
		return ranges_.pass([](basic_range auto&... ranges) {
			return __ranges::concat_view{ ranges... };
		});
	}

};

template<basic_range... Ranges>
ranges(Ranges&&...) -> ranges<Ranges...>;