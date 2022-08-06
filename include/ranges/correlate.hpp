#pragma once

#include "../range/basic.hpp"
#include "../loop_action.hpp"

enum class ranges_correlation_end_cause {
	first_ended, second_ended, both_ended, interrupted
};

template<basic_range FirstRange, basic_range SecondRange, typename Handler>
constexpr ranges_correlation_end_cause correlate_ranges(
	FirstRange&& first_range,
	SecondRange&& second_range,
	Handler&& handler
) {
	auto ai = range_iterator(first_range);
	auto bi = range_iterator(second_range);
	auto as = range_sentinel(first_range);
	auto bs = range_sentinel(second_range);

	while(true) {
		bool first  = ai == as;
		bool second = bi == bs;

		if( first &&  second) return ranges_correlation_end_cause::both_ended;
		if(!first &&  second) return ranges_correlation_end_cause::second_ended;
		if( first && !second) return ranges_correlation_end_cause::first_ended;

		loop_action action = handler(*ai++, *bi++);
		switch (action) {
			case loop_action::stop:
				return ranges_correlation_end_cause::interrupted;
			case loop_action::next:
				continue;
		}
	}
}