#pragma once

#include "../range/basic.hpp"
#include "../range/begin.hpp"
#include "../range/end.hpp"
#include "../move.hpp"
#include "../elements/of.hpp"
#include "../type/is_addition_assignable.hpp"

namespace ranges {

template<typename... Iterators>
class zip_view_iterator {
	elements::of<Iterators...> iterators_;
public:

	constexpr auto iterators() const { return iterators_; };

	constexpr zip_view_iterator(Iterators... iterators) :
		iterators_{ iterators... }
	{}

	constexpr auto operator * () {
		return iterators_.pass([](auto&... iterators) {
			return elements::of{ *iterators... };
		});
	}

	constexpr auto& operator ++ () {
		iterators_.pass([](auto&... iterators) {
			(++iterators, ...);
		});
		return *this;
	}

	constexpr auto& operator += (nuint n)
	requires (addition_assignable<Iterators, nuint> && ...) {
		iterators_.pass([&](auto&... iterators) {
			((iterators += n), ...);
		});
		return *this;
	}

	constexpr auto operator + (nuint n) const
	requires (addition_assignable<Iterators, nuint> && ...) {
		zip_view_iterator cpy{ *this };
		return cpy += n;
	}
};

template<typename... Sentinels>
class zip_view_sentinel {
	elements::of<Sentinels...> sentinels_;
public:

	constexpr auto sentinels() const { return sentinels_; }

	constexpr zip_view_sentinel(Sentinels... sentinels) :
		sentinels_{ sentinels... }
	{}
};

template<typename... Iterators, typename... Sentinels>
constexpr inline bool operator == (
	zip_view_iterator<Iterators...> it,
	zip_view_sentinel<Sentinels...> sentinel
) {
	return it.iterators().pass([&](auto... iterators) {
		return sentinel.sentinels().pass([&](auto... sentinels) {
			return ((iterators == sentinels) || ...);
		});
	});
}

template<typename... Iterators, typename... Sentinels>
constexpr inline bool operator == (
	zip_view_sentinel<Sentinels...> sentinel,
	zip_view_iterator<Iterators...> it
) { return it == sentinel; }

template<typename... Iterators, typename... Sentinels>
constexpr inline bool operator != (
	zip_view_iterator<Iterators...> it,
	zip_view_sentinel<Sentinels...> sentinel
) { return !(it == sentinel); }

template<typename... Iterators, typename... Sentinels>
constexpr inline bool operator != (
	zip_view_sentinel<Sentinels...> sentinel,
	zip_view_iterator<Iterators...> it
) { return it != sentinel; }

template<basic_range... Ranges>
class zip_view {
	elements::of<Ranges...> ranges_;

protected:

	constexpr zip_view(elements::of<Ranges...> ranges) :
		ranges_{ move(ranges) }
	{}

public:

	constexpr zip_view(Ranges&&... ranges) :
		ranges_{ forward<Ranges>(ranges)... }
	{}

	constexpr auto begin() const {
		return ranges_.pass([](auto&... ranges) {
			return zip_view_iterator{ range::begin(ranges)... };
		});
	}

	constexpr auto end() const {
		return ranges_.pass([](auto&... ranges) {
			return zip_view_sentinel{ range::end(ranges)... };
		});
	}

	constexpr auto operator [] (nuint n) const
	requires requires() { begin() + n; } {
		return *(begin() + n);
	}

};

template<basic_range... Ranges>
zip_view(Ranges&&...) -> zip_view<Ranges...>;

} // ranges