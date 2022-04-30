#pragma once

#include "range.hpp"
#include "begin.hpp"
#include "end.hpp"
#include "meta/elements/of.hpp"

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

	constexpr auto& operator += (nuint n) {
		iterators_.pass([&](auto&... iterators) {
			((iterators += n), ...);
		});
		return *this;
	}

	constexpr auto operator + (nuint n) {
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

template<range... Ranges>
class zip_view {
	elements::of<Ranges...> ranges_;

protected:

	template<typename... Ranges0>
	constexpr zip_view(elements::of<Ranges0...> ranges) :
		ranges_{ move(ranges) }
	{}

public:

	template<typename... Ranges0>
	constexpr zip_view(Ranges0&&... ranges) :
		ranges_{ forward<Ranges0>(ranges)... }
	{}

	constexpr auto begin() const {
		return ranges_.pass([](auto&... ranges) {
			return zip_view_iterator{ ::begin(ranges)... };
		});
	}

	constexpr auto end() const {
		return ranges_.pass([](auto&... ranges) {
			return zip_view_sentinel{ ::end(ranges)... };
		});
	}

	constexpr auto operator [] (nuint n) const {
		return *(begin() + n);
	}

};

template<range... Ranges>
zip_view(Ranges&&...) -> zip_view<Ranges...>;