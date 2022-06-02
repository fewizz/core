#pragma once

#include "range.hpp"
#include "begin.hpp"
#include "end.hpp"
#include "value_type.hpp"
#include "default_sentinel.hpp"
#include "meta/elements/of.hpp"
#include "meta/types/common.hpp"

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
	constexpr types::common::for_types<
		decltype(*declval<typename Pairs::template type_at<0>>())...
	>
	deref() const {
		if(index_ == Index) {
			return *pairs_.template at<Index>().template at<0>();
		}
		else if constexpr(Index + 1 < sizeof...(Pairs)) {
			return deref<Index + 1>();
		}
		__builtin_unreachable();
	}

	template<nuint Index = 0>
	constexpr types::common::for_types<
		decltype(*declval<typename Pairs::template type_at<0>>())...
	> deref() {
		if(index_ == Index) {
			return *pairs_.template at<Index>().template at<0>();
		}
		else if constexpr(Index + 1 < sizeof...(Pairs)) {
			return deref<Index + 1>();
		}
		__builtin_unreachable();
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

	constexpr auto operator ++ (int) {
		concat_view_iterator cpy{ *this };
		++*this;
		return cpy;
	}

	constexpr auto& operator += (nuint n) {
		while(n > 0) {
			inc();
			--n;
		}
		return *this;
	}

	constexpr auto operator + (nuint n) const {
		concat_view_iterator cpy{ *this };
		return cpy += n;
	}

	constexpr auto operator - (concat_view_iterator other) const {
		return index_ - other.index_;
	}

	constexpr bool operator == (concat_view_iterator other) const {
		return index_ == other.index_;
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

	constexpr concat_view(Ranges&&... ranges) :
		ranges_{ forward<Ranges>(ranges)... }
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

	constexpr nuint size() const {
		return ranges_.pass([](auto&&... ranges) {
			return (ranges.size() + ...);
		});
	}
};

template<range... Ranges>
concat_view(Ranges&&...) -> concat_view<Ranges...>;