#pragma once

#include "../__range/basic.hpp"
#include "../__range/iterator.hpp"
#include "../__range/sentinel.hpp"
#include "../__range/default_sentinel.hpp"
#include "../__iterator/element_type.hpp"
#include "../iterators/distance.hpp"
#include "../forward.hpp"
#include "../elements/of.hpp"
#include "../types/common.hpp"
#include "../expression_of_type.hpp"

namespace __ranges {

template<typename... Pairs>
class concat_view_iterator {
	elements::of<Pairs...> pairs_;
	nuint index_ = 0;

	template<typename Pair>
	using pair_iterator_type = typename Pair::template type_at<0>;

	using element_type = types::common::for_types<
		iterator_element_type<pair_iterator_type<Pairs>>...
	>;

	template<nuint Index>
	static constexpr bool has_next = Index + 1 < sizeof...(Pairs);

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		if(FromIndex == index_) {
			return handler.template operator () <FromIndex>(
				pairs_.template at<FromIndex>()
			);
		}
		if constexpr(has_next<FromIndex>) {
			return current_pair_with_index<FromIndex + 1>(
				forward<Handler>(handler)
			);
		}
		// ???
		__builtin_unreachable();
	}

	template<nuint FromIndex = 0, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 1)
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		return handler.template operator() <FromIndex>(
			pairs_.template at<FromIndex>()
		);
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 2)
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		switch (index_) {
			case FromIndex + 0: return handler.template operator()
				<FromIndex + 0>(pairs_.template at<FromIndex + 0>());
			case FromIndex + 1: return handler.template operator()
				<FromIndex + 1>(pairs_.template at<FromIndex + 1>());
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 3)
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		switch (index_) {
			case FromIndex + 0: return handler.template operator()
				<FromIndex + 0>(pairs_.template at<FromIndex + 0>());
			case FromIndex + 1: return handler.template operator()
				<FromIndex + 1>(pairs_.template at<FromIndex + 1>());
			case FromIndex + 2: return handler.template operator()
				<FromIndex + 2>(pairs_.template at<FromIndex + 2>());
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 4)
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		switch (index_) {
			case FromIndex + 0: return handler.template operator()
				<FromIndex + 0>(pairs_.template at<FromIndex + 0>());
			case FromIndex + 1: return handler.template operator()
				<FromIndex + 1>(pairs_.template at<FromIndex + 1>());
			case FromIndex + 2: return handler.template operator()
				<FromIndex + 2>(pairs_.template at<2>());
			case FromIndex + 3: return handler.template operator()
				<FromIndex + 3>(pairs_.template at<3>());
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex == 5)
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		switch (index_) {
			case FromIndex + 0: return handler.template operator()
				<FromIndex + 0>(pairs_.template at<FromIndex + 0>());
			case FromIndex + 1: return handler.template operator()
				<FromIndex + 1>(pairs_.template at<FromIndex + 1>());
			case FromIndex + 2: return handler.template operator()
				<FromIndex + 2>(pairs_.template at<FromIndex + 2>());
			case FromIndex + 3: return handler.template operator()
				<FromIndex + 3>(pairs_.template at<FromIndex + 3>());
			case FromIndex + 4: return handler.template operator()
				<FromIndex + 4>(pairs_.template at<4>());
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex, typename Handler>
	requires(sizeof...(Pairs) - FromIndex== 6)
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) {
		switch (index_) {
			case FromIndex + 0: return handler.template operator()
				<FromIndex + 0>(pairs_.template at<FromIndex + 0>());
			case FromIndex + 1: return handler.template operator()
				<FromIndex + 1>(pairs_.template at<FromIndex + 1>());
			case FromIndex + 2: return handler.template operator()
				<FromIndex + 2>(pairs_.template at<FromIndex + 2>());
			case FromIndex + 3: return handler.template operator()
				<FromIndex + 3>(pairs_.template at<FromIndex + 3>());
			case FromIndex + 4: return handler.template operator()
				<FromIndex + 4>(pairs_.template at<FromIndex + 4>());
			case FromIndex + 5: return handler.template operator()
				<FromIndex + 5>(pairs_.template at<FromIndex + 5>());
		}
		__builtin_unreachable();
	}

	template<nuint FromIndex = 0, typename Handler>
	constexpr decltype(auto) current_pair_with_index(Handler&& handler) const {
		return ((concat_view_iterator&) *this)
			.current_pair_with_index<FromIndex>(
				[&]<nuint Index, typename PairType>(PairType& pair)
				-> decltype(auto) {
					return handler.template
						operator () <Index>((const PairType&) pair);
				}
			);
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

	static constexpr basic_iterator decltype(auto) it(auto& pair) {
		return pair.template at<0>();
	}

	static constexpr decltype(auto) end(auto& pair) {
		return pair.template at<1>();
	}

	template<nuint FromIndex, typename Handler>
	constexpr void skip_empty(Handler&& handler) {
		current_pair_with_index<FromIndex>([&]<nuint Index>(auto& pair) {
			if constexpr(has_next<Index>) {
				if(it(pair) == end(pair)) {
					++index_;
					skip_empty<Index + 1>(forward<Handler>(handler));
					return;
				}
			}
			handler.template operator () <Index>();
		});
	}

	template<nuint FromIndex, typename Handler>
	requires(FromIndex + 1 == sizeof...(Pairs))
	constexpr void skip_empty(Handler&& handler) {
		handler.template operator () <FromIndex>();
	}

	template<nuint FromIndex, typename Handler>
	requires(FromIndex + 2 == sizeof...(Pairs))
	constexpr void skip_empty(Handler&& handler) {
		auto& pair = pairs_.template at<FromIndex>();
		if(it(pair) == end(pair)) {
			++index_;
			handler.template operator () <FromIndex + 1>();
			return;
		}
		handler.template operator () <FromIndex>();
	}

	template<nuint FromIndex, typename Handler>
	requires(FromIndex + 3 == sizeof...(Pairs))
	constexpr void skip_empty(Handler&& handler) {
		auto& pair = pairs_.template at<FromIndex>();
		if(it(pair) == end(pair)) {
			++index_;
			auto& pair = pairs_.template at<FromIndex + 1>();
			if(it(pair) == end(pair)) {
				++index_;
				handler.template operator () <FromIndex + 2>();
				return;
			}
			handler.template operator () <FromIndex + 1>();
			return;
		}
		handler.template operator () <FromIndex>();
	}

	template<nuint FromIndex, typename Handler>
	requires(FromIndex + 4 == sizeof...(Pairs))
	constexpr void skip_empty(Handler&& handler) {
		auto& pair = pairs_.template at<FromIndex>();
		if(it(pair) == end(pair)) {
			++index_;
			auto& pair = pairs_.template at<FromIndex + 1>();
			if(it(pair) == end(pair)) {
				++index_;
				auto& pair = pairs_.template at<FromIndex + 2>();
				if(it(pair) == end(pair)) {
					++index_;
					handler.template operator () <FromIndex + 3>();
					return;
				}
				handler.template operator () <FromIndex + 2>();
				return;
			}
			handler.template operator () <FromIndex + 1>();
			return;
		}
		handler.template operator () <FromIndex>();
	}

	template<nuint FromIndex, typename Handler>
	requires(FromIndex + 5 == sizeof...(Pairs))
	constexpr void skip_empty(Handler&& handler) {
		auto& pair = pairs_.template at<FromIndex>();
		if(it(pair) == end(pair)) {
			++index_;
			auto& pair = pairs_.template at<FromIndex + 1>();
			if(it(pair) == end(pair)) {
				++index_;
				auto& pair = pairs_.template at<FromIndex + 2>();
				if(it(pair) == end(pair)) {
					++index_;
					auto& pair = pairs_.template at<FromIndex + 3>();
					if(it(pair) == end(pair)) {
						++index_;
						handler.template operator () <FromIndex + 4>();
						return;
					}
					handler.template operator () <FromIndex + 3>();
					return;
				}
				handler.template operator () <FromIndex + 2>();
				return;
			}
			handler.template operator () <FromIndex + 1>();
			return;
		}
		handler.template operator () <FromIndex>();
	}

	template<nuint FromIndex, typename Handler>
	requires(FromIndex + 6 == sizeof...(Pairs))
	constexpr void skip_empty(Handler&& handler) {
		auto& pair = pairs_.template at<FromIndex>();
		if(it(pair) == end(pair)) {
			++index_;
			auto& pair = pairs_.template at<FromIndex + 1>();
			if(it(pair) == end(pair)) {
				++index_;
				auto& pair = pairs_.template at<FromIndex + 2>();
				if(it(pair) == end(pair)) {
					++index_;
					auto& pair = pairs_.template at<FromIndex + 3>();
					if(it(pair) == end(pair)) {
						++index_;
						auto& pair = pairs_.template at<FromIndex + 4>();
						if(it(pair) == end(pair)) {
							++index_;
							handler.template operator () <FromIndex + 5>();
							return;
						}
						handler.template operator () <FromIndex + 4>();
						return;
					}
					handler.template operator () <FromIndex + 3>();
					return;
				}
				handler.template operator () <FromIndex + 2>();
				return;
			}
			handler.template operator () <FromIndex + 1>();
			return;
		}
		handler.template operator () <FromIndex>();
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
			n -= len;
			skip_empty<Index>([=]<nuint Index0>() {
				if constexpr (Index0 > Index) {
					if(n > 0) {
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
							iterators_distance(
								it(other.pairs_.template at<Indices>()),
								end(other.pairs_.template at<Indices>())
							)
							+ ... + 0
						);
						len +=
							iterators_distance(
								it(other.pairs_.template at<Index>()),
								it(pair)
							);
						return len;
					}(typename indices::from<OtherIndex>::template to<Index>{});
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

	constexpr concat_view_iterator(Pairs... pairs) : pairs_{ pairs... } {
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

template<basic_range... Ranges>
class concat_view {
	elements::of<Ranges...> ranges_;
public:

	constexpr concat_view(Ranges&&... ranges) :
		ranges_{ forward<Ranges>(ranges)... }
	{}

	constexpr auto iterator() const {
		return ranges_.pass([](auto&&... ranges) {
			return concat_view_iterator {
				elements::of {
					range_iterator(ranges),
					range_sentinel(ranges)
				} ...
			};
		});
	}

	constexpr auto sentinel() const {
		return default_sentinel{};
	}

	constexpr nuint size() const {
		return ranges_.pass([](auto&&... ranges) {
			return (ranges.size() + ...);
		});
	}

};

template<basic_range... Ranges>
concat_view(Ranges&&...) -> concat_view<Ranges...>;

} // __ranges