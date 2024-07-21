#pragma once

#include "../__range/basic.hpp"
#include "../__range/iterator.hpp"
#include "../__range/sentinel.hpp"
#include "../__range/default_sentinel.hpp"
#include "../__range/size.hpp"
#include "../__range/extensions.hpp"
#include "../__iterator/element_type.hpp"
#include "../__iterator/random_access.hpp"
#include "../forward.hpp"
#include "../tuple.hpp"
#include "../__types/common.hpp"

namespace __ranges {

template<typename... Pairs>
class concat_view_iterator {
	tuple<Pairs...> pairs_;
	nuint index_ = 0;

	template<typename Pair>
	using pair_iterator_type = typename Pair::template type_at<0>;

	using common_element_type = __types::common::for_types<
		iterator_element_type<pair_iterator_type<Pairs>>...
	>;

	template<nuint Index>
	static constexpr bool has_next = Index + 1 < sizeof...(Pairs);

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		__builtin_assume(index_ < sizeof...(Pairs));
		if (FromIndex == index_) {
			return handler.template operator () <FromIndex>();
		}
		if constexpr(has_next<FromIndex>) {
			return current_pair_index<FromIndex + 1>(
				forward<Handler>(handler)
			);
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex = 0, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 1)
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		return handler.template operator() <FromIndex + 0>();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 2)
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		switch (index_) {
			case FromIndex + 0:
				return handler.template operator() <FromIndex + 0>();
			case FromIndex + 1:
				return handler.template operator() <FromIndex + 1>();
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 3)
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		switch (index_) {
			case FromIndex + 0:
				return handler.template operator() <FromIndex + 0>();
			case FromIndex + 1:
				return handler.template operator() <FromIndex + 1>();
			case FromIndex + 2:
				return handler.template operator() <FromIndex + 2>();
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 4)
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		switch (index_) {
			case FromIndex + 0:
				return handler.template operator() <FromIndex + 0>();
			case FromIndex + 1:
				return handler.template operator() <FromIndex + 1>();
			case FromIndex + 2:
				return handler.template operator() <FromIndex + 2>();
			case FromIndex + 3:
				return handler.template operator() <FromIndex + 3>();
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 5)
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		switch (index_) {
			case FromIndex + 0:
				return handler.template operator() <FromIndex + 0>();
			case FromIndex + 1:
				return handler.template operator() <FromIndex + 1>();
			case FromIndex + 2:
				return handler.template operator() <FromIndex + 2>();
			case FromIndex + 3:
				return handler.template operator() <FromIndex + 3>();
			case FromIndex + 4:
				return handler.template operator() <FromIndex + 4>();
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex== 6)
	constexpr decltype(auto) current_pair_index(Handler&& handler) const {
		switch (index_) {
			case FromIndex + 0:
				return handler.template operator() <FromIndex + 0>();
			case FromIndex + 1:
				return handler.template operator() <FromIndex + 1>();
			case FromIndex + 2:
				return handler.template operator() <FromIndex + 2>();
			case FromIndex + 3:
				return handler.template operator() <FromIndex + 3>();
			case FromIndex + 4:
				return handler.template operator() <FromIndex + 4>();
			case FromIndex + 5:
				return handler.template operator() <FromIndex + 5>();
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair(this auto&& self, Handler&& handler) {
		return self.current_pair_index([&]<nuint Index>()
		-> decltype(auto) {
			return handler(self.pairs_.template get<Index>());
		});
	}

	static constexpr basic_iterator auto& it(auto& pair) {
		return pair.template get<0>();
	}

	static constexpr auto& end(auto& pair) {
		return pair.template get<1>();
	}

	template<nuint CurrentIndex>
	constexpr void skip_empty() {
		if constexpr(has_next<CurrentIndex>) {
			auto& pair = pairs_.template get<CurrentIndex>();
			if (it(pair) == end(pair)) {
				++index_;
				skip_empty<CurrentIndex + 1>();
				return;
			}
		}
	}

public:

	constexpr concat_view_iterator(Pairs... pairs) : pairs_{ pairs... } {
		skip_empty<0>();
	}

	constexpr bool is_ended() {
		auto& pair = pairs_.template get<sizeof...(Pairs) - 1>();
		return
			index_ == sizeof...(Pairs) - 1 &&
			it(pair) == end(pair);
	}

	constexpr common_element_type operator * (this auto&& self) {
		return self.current_pair(
			[&](auto& pair) -> common_element_type { return *it(pair); }
		);
	}

	constexpr concat_view_iterator& operator ++ () {
		current_pair_index([&]<nuint Index>() {
			auto& pair = pairs_.template get<Index>();
			++it(pair);
			skip_empty<Index>();
		});
		return *this;
	}

	constexpr concat_view_iterator operator ++ (int) {
		concat_view_iterator cpy = *this;
		++*this;
		return cpy;
	}

	constexpr concat_view_iterator& operator += (nuint n) {
		if (n > 0) {
			current_pair_index([&]<nuint Index>() {
				auto& pair = pairs_.template get<Index>();
				nuint len = end(pair) - it(pair);
				if (n < len) {
					it(pair) += n;
					return;
				}
				it(pair) += len;
				n -= len;
				skip_empty<Index>();
				(*this) += n;
			});
		}
		return *this;
	}

	constexpr concat_view_iterator operator + (nuint n) const {
		concat_view_iterator cpy{ *this };
		return cpy += n;
	}

	constexpr auto operator - (concat_view_iterator other) const
	requires (random_access_iterator<pair_iterator_type<Pairs>> && ...) {
		return other.current_pair_index([&]<nuint OtherIndex>() {
		return this->current_pair_index<OtherIndex>([&]<nuint Index>() {
			nuint dist = other.pairs_.pass(
				[&](auto&... pairs) {
					return ((end(pairs) - it(pairs)) + ... + 0);
				},
				typename indices::from<OtherIndex>::template to<Index>{}
			);
			auto& pair = pairs_.template get<Index>();
			dist += it(pair) - it(other.pairs_.template get<Index>());
			return dist;
		});});
	}

	friend constexpr auto
	operator - (default_sentinel, concat_view_iterator iterator)
	requires (random_access_iterator<pair_iterator_type<Pairs>> && ...) {
		return iterator.pairs_.pass([&](auto&... pairs) {
			return ((end(pairs) - it(pairs)) + ...);
		});
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

	constexpr friend bool operator == (
		__ranges::concat_view_iterator<Pairs...> it, default_sentinel
	) { return it.is_ended(); }

	constexpr friend bool operator == (
		default_sentinel, __ranges::concat_view_iterator<Pairs...> it
	) { return it.is_ended(); }

	constexpr friend bool operator != (
		__ranges::concat_view_iterator<Pairs...> it, default_sentinel
	) { return !it.is_ended(); }

	constexpr friend bool operator != (
		default_sentinel, __ranges::concat_view_iterator<Pairs...> it
	) { return !it.is_ended(); }

};

template<basic_range... Ranges>
class concat_view : public range_extensions<concat_view<Ranges...>> {
	tuple<Ranges...> ranges_;
public:

	constexpr concat_view(Ranges&&... ranges) :
		ranges_{ forward<Ranges>(ranges)... }
	{}

	constexpr auto iterator() const {
		return ranges_.pass([](const Ranges&... ranges) {
			return concat_view_iterator {
				tuple {
					range_iterator(ranges), range_sentinel(ranges)
				} ...
			};
		});
	}
	constexpr auto iterator()       {
		return ranges_.pass([](Ranges&... ranges) {
			return concat_view_iterator {
				tuple {
					range_iterator(ranges), range_sentinel(ranges)
				} ...
			};
		});
	}

	constexpr auto sentinel() const {
		return default_sentinel{};
	}

	constexpr nuint size() const requires ((sized_range<Ranges> && ...)) {
		return ranges_.pass([](const Ranges&... ranges) {
			return (((nuint) range_size(ranges)) + ...);
		});
	}

};

template<basic_range... Ranges>
concat_view(Ranges&&...) -> concat_view<Ranges...>;

} // __ranges