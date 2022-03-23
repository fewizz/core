#pragma once

#include "basic.hpp"
#include "of_value_type_same_as.hpp"
#include "default_sentinel.hpp"
#include "sub.hpp"
#include "contains.hpp"
#include "../array.hpp"

namespace range {

	// TODO range::of<iterator::value_type<>> SplitRange
	template<typename It, typename Sentinel, typename SplitRange>
	struct splitted_iterator {
		It begin;
		It end;
		Sentinel base_iterator_end;
		SplitRange& split_range;

		constexpr splitted_iterator(
			It base_iterator_begin,
			Sentinel base_iterator_end,
			SplitRange& split_range
		) :
			begin{ base_iterator_begin },
			end{ base_iterator_begin },
			base_iterator_end{ base_iterator_end },
			split_range{ split_range }
		{
			while(
				begin != base_iterator_end &&
				range::contains(*begin).in(split_range)
			) { ++begin; }
			end = begin;
			if(end != base_iterator_end) ++end;
			while(
				end != base_iterator_end &&
				!range::contains(*end).in(split_range)
			) { ++end; }
		}

		using value_type = range::sub<It, It>;

		constexpr auto& operator ++ () {
			begin = end;
			while(
				begin != base_iterator_end &&
				range::contains(*begin).in(split_range)
			) { ++begin; }
			end = begin;
			if(end != base_iterator_end) ++end;
			while(
				end != base_iterator_end &&
				!range::contains(*end).in(split_range)
			) { ++end; }

			return *this;
		}

		constexpr auto& operator += (nuint n) {
			for(nuint i = 0; i < n; ++i) ++(*this);
			return *this;
		}

		constexpr auto operator + (nuint n) const {
			return splitted_iterator{ *this } += n;
		}

		constexpr value_type operator * () const {
			return{ begin, end };
		}
	};

	template<typename It, typename Sentinel, typename SplitRange>
	inline bool operator == (
		splitted_iterator<It, Sentinel, SplitRange> it,
		default_sentinel
	) {
		return it.begin == it.base_iterator_end;
	}

	template<range::basic Range, range::of<range::value_type<Range>> SplitRange>
	struct splitted {
		Range& base_range;
		SplitRange split_range;

		constexpr splitted(Range& base_range, SplitRange split_range) :
			base_range{ base_range },
			split_range{ forward<SplitRange>(split_range) }
		{}

		constexpr auto begin() {
			return splitted_iterator {
				base_range.begin(),
				base_range.end(),
				split_range
			};
		}

		constexpr auto end() { return default_sentinel{}; }

		constexpr decltype(auto) operator [] (nuint index) const {
			return *(begin() + index);
		}

		constexpr decltype(auto) operator [] (nuint index) {
			return *(begin() + index);
		}
	};

	template<range::basic Range>
	struct split {
		Range& range;

		constexpr split(Range& range) : range{ range } {}

		template<range::of<range::value_type<Range>> SplitRange>
		constexpr auto with(SplitRange split_range) {
			return splitted{ range, move(split_range) };
		}

		template<typename... Args>
		requires types_are_same<range::value_type<Range>, Args...>
		constexpr auto with(Args... splitters) {
			return with(array{ splitters... });
		}
	};

} // range