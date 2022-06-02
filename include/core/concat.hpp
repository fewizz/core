#pragma once

#include "range.hpp"
#include "begin.hpp"
#include "end.hpp"
#include "value_type.hpp"
#include "forward.hpp"
#include "default_sentinel.hpp"
#include "meta/elements/of.hpp"
#include "meta/types/common.hpp"

template<typename... Pairs>
class concat_view_iterator {
	elements::of<Pairs...> pairs_;
	nuint index_ = 0;

	using element_type = types::common::for_types<
		decltype(*declval<typename Pairs::template type_at<0>>())...
	>;

	template<nuint Index>
	static constexpr bool there_is_next = Index + 1 < sizeof...(Pairs);

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) const {
		if(FromIndex == index_) {
			return handler.template operator () <FromIndex>(
				pairs_.template at<FromIndex>()
			);
		}
		if constexpr(there_is_next<FromIndex>) {
			return current_pair_with_index<FromIndex + 1>(
				forward<Handler>(handler)
			);
		}
		// ???
		__builtin_unreachable();
	}

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		if(FromIndex == index_) {
			return handler.template operator () <FromIndex>(
				pairs_.template at<FromIndex>()
			);
		}
		if constexpr(there_is_next<FromIndex>) {
			return current_pair_with_index<FromIndex + 1>(
				forward<Handler>(handler)
			);
		}
		// ???
		__builtin_unreachable();
	}

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair(Handler&& handler) const {
		return current_pair_with_index([&]<nuint Index>(auto& pair) {
			return handler(pair);
		});
	}

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair(Handler&& handler) {
		return current_pair_with_index([&]<nuint Index>(auto& pair) {
			return handler(pair);
		});
	}

	static constexpr decltype(auto) it(auto& pair) {
		return pair.template at<0>();
	}

	static constexpr decltype(auto) end(auto& pair) {
		return pair.template at<1>();
	}

	template<nuint FromIndex, typename Handler>
	constexpr void skip_empty(Handler&& handler) {
		current_pair_with_index<FromIndex>([&]<nuint Index>(auto& pair) {
			if constexpr(there_is_next<Index>) {
				if(it(pair) == end(pair)) {
					++index_;
					skip_empty<Index + 1>(forward<Handler>(handler));
					return;
				}
			}
			handler.template operator () <Index>();
		});
	}

	template<nuint FromIndex = 0>
	constexpr void skip_empty() {
		return skip_empty<FromIndex>([]<nuint Index>{});
	}

	constexpr element_type deref() const {
		return current_pair(
			[&](auto& pair) -> element_type { return *it(pair); }
		);
	}

	constexpr element_type deref() {
		return current_pair(
			[&](auto& pair) -> element_type { return *it(pair); }
		);
	}

	constexpr void inc() {
		current_pair_with_index([&]<nuint Index>(auto& pair) {
			++it(pair);
			skip_empty<Index>();
		});
	}

	template<nuint FromIndex = 0>
	constexpr void inc(nuint n) {
		current_pair_with_index<FromIndex>([&]<nuint Index>(auto& pair) {
			nuint len = end(pair) - it(pair);
			if(n < len) {
				it(pair) += n;
				return;
			}
			it(pair) += len;
			skip_empty<Index>([&]<nuint Index0>() {
				if(n > len) {
					if constexpr(there_is_next<Index0 - 1>) {
						n -= len;
						inc<Index0>(n);
					}
				}
			});
		});
	}

	constexpr nuint dist(concat_view_iterator other) const {
		return other.current_pair_with_index([&]<nuint OtherIndex>(auto&) {
			return current_pair_with_index<OtherIndex>(
				[&]<nuint Index>(auto& pair) {
					return [&]<nuint... Indices>(indices::of<Indices...>) {
						nuint len = (
							(
								end(other.pairs_.template at<Indices>()) -
								it(other.pairs_.template at<Indices>())
							)
							+ ... + 0
						);
						len +=
							it(pair) -
							it(other.pairs_.template at<Index>());
						return len;
					}(
						typename indices::
							from<OtherIndex>::template
							to<Index>{}
					);
				}
			);
		});
	}

public:

	constexpr bool is_ended() {
		auto& pair = pairs_.template at<sizeof...(Pairs) - 1>();
		return
			index_ == sizeof...(Pairs) - 1 &&
			it(pair) == end(pair);
	}

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
		inc(n);
		return *this;
	}

	constexpr auto operator + (nuint n) const {
		concat_view_iterator cpy{ *this };
		return cpy += n;
	}

	constexpr auto operator - (concat_view_iterator other) const {
		return dist(other);
	}

	constexpr bool operator == (concat_view_iterator other) const {
		return current_pair([&](auto& pair) {
			return other.current_pair([&](auto& other_pair) {
				return
					index_ == other.index_ &&
					it(pair) == it(other_pair);
			});
		});
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
//requires types_are_same<value_type<Ranges>...>
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