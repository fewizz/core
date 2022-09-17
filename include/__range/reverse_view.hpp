#pragma once

#include "./sized.hpp"
#include "./size.hpp"
#include "./extensions.hpp"
#include "../__iterator/basic.hpp"
#include "../forward.hpp"

namespace __range {

template<basic_iterator Iterator>
struct reverse_view_iterator {
private:
	Iterator iterator_;
public:

	constexpr reverse_view_iterator(Iterator iterator) : iterator_{ iterator } {}

	constexpr decltype(auto) operator * () const {
		Iterator cpy = iterator_;
		--cpy;
		return *cpy;
	}
	constexpr decltype(auto) operator * () {
		Iterator cpy = iterator_;
		--cpy;
		return *cpy;
	}

	constexpr auto& operator ++ () { --iterator_; return *this; }
	constexpr auto& operator -- () { ++iterator_; return *this; }
	constexpr auto& operator += (nuint n) { iterator_ -= n; return *this; }
	constexpr auto& operator -= (nuint n) { iterator_ += n; return *this; }

	constexpr bool operator == (reverse_view_iterator other) const {
		return iterator_ == other.iterator_;
	}

	friend constexpr bool operator == (
		reverse_view_iterator a, Iterator b
	) { return a.iterator_ == b; }
	friend constexpr bool operator == (
		Iterator a, reverse_view_iterator b
	) { return a == b.iterator_; }

	constexpr auto operator + (nuint n) const {
		return reverse_view_iterator{ *this } += n;
	}

};

template<sized_range Range>
struct reverse_view : range_extensions<reverse_view<Range>> {
private:
	Range range_;
public:

	constexpr reverse_view(Range&& range): range_{ forward<Range>(range) } {}

	constexpr auto iterator() {
		auto it   = range_iterator(range_);
		auto size = range_size(range_);
		// avoid applying non-zero offset  to null pointer
		return reverse_view_iterator {
			size == 0 ? it : it + size
		};
	}

	constexpr auto sentinel() {
		return reverse_view_iterator {
			range_iterator(range_)
		};
	}

	constexpr auto size() {
		return range_size(range_);
	}

};

template<sized_range Range>
reverse_view(Range&&) -> reverse_view<Range>;

} // __range