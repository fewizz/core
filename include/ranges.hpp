#pragma once

#include "./__range/basic.hpp"
#include "./__ranges/have_equal_size_and_elements.hpp"
#include "./__ranges/concat_view.hpp"
#include "./__ranges/transform_view.hpp"
#include "./tuple.hpp"

template<basic_range... Ranges>
struct ranges {
private:
	tuple<Ranges...> ranges_;
public:

	constexpr ranges(Ranges&&... ranges) :
		ranges_{ forward<Ranges>(ranges)... }
	{}

	constexpr bool have_equal_size_and_elements() const {
		return ranges_.pass([](const Ranges&... ranges) {
			return __ranges::have_equal_size_and_elements(ranges...);
		});
	}

	constexpr auto concat_view() const {
		return ranges_.pass([](const Ranges&... ranges) {
			return __ranges::concat_view{ ranges... };
		});
	}
	constexpr auto concat_view() {
		return ranges_.pass([](      Ranges&... ranges) {
			return __ranges::concat_view{ ranges... };
		});
	}
	
	template<typename Function>
	constexpr auto transform_view(Function&& function) const {
		return ranges_.pass([&](const Ranges&... ranges) {
			return __ranges::transform_view {
				forward<Function>(function), ranges...
			};
		});
	}
	template<typename Function>
	constexpr auto transform_view(Function&& function)       {
		return ranges_.pass([&](Ranges&... ranges) {
			return __ranges::transform_view {
				forward<Function>(function), ranges...
			};
		});
	}

};

template<basic_range... Ranges>
ranges(Ranges&&...) -> ranges<Ranges...>;