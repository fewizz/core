#pragma once

#include "range.hpp"
#include "begin.hpp"
#include "end.hpp"
#include "value_type.hpp"
#include "default_sentinel.hpp"
#include "meta/elements/of.hpp"

template<typename... Pairs>
class concat_view_iterator {
	elements::of<Pairs...> pairs_;
	nuint index_ = 0;

	template<nuint Index = 0>
	constexpr void skip_empty() {
		if(Index == index_) {
			auto& p = pairs_.template at<Index>();
			if(p.template at<0>() == p.template at<1>()) {
				++index_;
			}
		}
		else if (Index > index_) return;

		if constexpr(Index + 1 < sizeof...(Pairs)) skip_empty<Index + 1>();
	}

	template<nuint Index = 0>
	constexpr decltype(auto) deref() const {
		if(index_ == Index) {
			return *pairs_.template at<Index>().template at<0>();
		}
		else if constexpr(Index + 1 < sizeof...(Pairs)) {
			return deref<Index + 1>();
		}
		throw;
	}

	template<nuint Index = 0>
	constexpr decltype(auto) deref() {
		if(index_ == Index) {
			return *pairs_.template at<Index>().template at<0>();
		}
		else if constexpr(Index + 1 < sizeof...(Pairs)) {
			return deref<Index + 1>();
		}
		throw;
	}

	template<nuint Index = 0>
	constexpr void inc() {
		if(index_ == Index) {
			++pairs_.template at<Index>().template at<0>();
			skip_empty<Index>();
			return;
		}
		if constexpr(Index + 1 < sizeof...(Pairs)) inc<Index + 1>();
	}

public:

	constexpr bool is_ended() { return index_ == sizeof...(Pairs); }

	constexpr concat_view_iterator(Pairs... pairs) :
		pairs_{ pairs... }
	{
		skip_empty();
	}

	constexpr decltype(auto) operator * () const { return deref(); }
	constexpr decltype(auto) operator * () { return deref(); }

	constexpr auto& operator ++ () {
		inc();
		return *this;
	}

};

template<typename... Pairs>
constexpr bool operator == (
	concat_view_iterator<Pairs...> it, default_sentinel
) { return it.is_ended(); }

template<typename... Pairs>
constexpr bool operator == (
	default_sentinel, concat_view_iterator<Pairs...> it
) { return it.is_ended(); }

template<typename... Pairs>
constexpr bool operator != (
	concat_view_iterator<Pairs...> it, default_sentinel
) { return !it.is_ended(); }

template<typename... Pairs>
constexpr bool operator != (
	default_sentinel, concat_view_iterator<Pairs...> it
) { return !it.is_ended(); }

template<range... Ranges>
requires types_are_same<value_type<Ranges>...>
class concat_view {
	elements::of<Ranges...> ranges_;
public:

	template<range... Ranges0>
	constexpr concat_view(Ranges0... ranges) :
		ranges_{ forward<Ranges0>(ranges)... }
	{}

	constexpr auto begin() const {
		return ranges_.pass([](auto&&... ranges) {
			return concat_view_iterator {
				elements::of{ ::begin(ranges), ::end(ranges) } ...
			};
		});
	}

	constexpr auto end() const {
		return default_sentinel{};
	}
};

template<range... Ranges>
concat_view(Ranges&&...) -> concat_view<Ranges...>;