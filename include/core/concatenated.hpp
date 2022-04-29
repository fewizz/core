#pragma once

#include "range.hpp"
#include "value_type.hpp"
#include "default_sentinel.hpp"

template<
	typename FirstIterator,  typename FirstSentinel,
	typename SecondIterator, typename SecondSentinel
>
struct concatenated_iterator {
	FirstIterator first_iterator;
	FirstSentinel first_sentinel;

	SecondIterator second_iterator;
	SecondSentinel second_sentinel;

	concatenated_iterator(
		FirstIterator fi,  FirstSentinel fs,
		SecondIterator si, SecondSentinel ss
	) :
		 first_iterator{ fi },  first_sentinel{ fs },
		second_iterator{ si }, second_sentinel{ ss }
	{}

	decltype(auto) operator * () const {
		if(first_iterator != first_sentinel) {
			return *first_iterator;
		}
		return *second_iterator;
	}

	decltype(auto) operator * () {
		if(first_iterator != first_sentinel) {
			return *first_iterator;
		}
		return *second_iterator;
	}

	auto& operator ++ () {
		if(first_iterator != first_sentinel) {
			++first_iterator;
		} else {
			++second_iterator;
		}
		return *this;
	}
};

template<
	typename  FirstIterator, typename  FirstSentinel,
	typename SecondIterator, typename SecondSentinel
>
bool operator == (
	concatenated_iterator<
		 FirstIterator,  FirstSentinel,
		SecondIterator, SecondSentinel
	> it,
	default_sentinel
) {
	return it. first_iterator == it. first_sentinel &&
	       it.second_iterator == it.second_sentinel;
}

template<
	typename  FirstIterator, typename  FirstSentinel,
	typename SecondIterator, typename SecondSentinel
>
bool operator == (
	default_sentinel,
	concatenated_iterator<
		 FirstIterator,  FirstSentinel,
		SecondIterator, SecondSentinel
	> it
) {
	return it == default_sentinel{};
}

template<range FirstRange, range SecondRange>
requires types_are_same<value_type<FirstRange>, value_type<SecondRange>>
class concatenated {
	FirstRange& first_range;
	SecondRange& second_range;
public:

	using iterator_type = concatenated_iterator<
		decay<decltype(first_range.begin())>,
		decay<decltype(first_range.end())>,
		decay<decltype(second_range.begin())>,
		decay<decltype(second_range.end())>
	>;

	concatenated(FirstRange& first, SecondRange& second) :
		first_range{ first },
		second_range{ second }
	{}

	iterator_type begin() {
		return {
			first_range.begin(),  first_range.end(),
			second_range.begin(), second_range.end()
		};
	}

	default_sentinel end() const {
		return {};
	}

	nuint size() const {
		return first_range.size() + second_range.size();
	}

};