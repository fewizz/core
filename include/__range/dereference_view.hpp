#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "./extensions.hpp"
#include "../forward.hpp"
#include "../__type/is_pointer.hpp"
#include "../__iterator/element_type.hpp"

namespace __range {

template<basic_iterator Iterator>
requires type_is_pointer<decay<iterator_element_type<Iterator>>>
class dereference_iterator {
	Iterator iterator_;
public:

	constexpr dereference_iterator(Iterator iterator) :
		iterator_{ iterator }
	{}

	constexpr auto& operator * () const { return ** iterator_; }
	constexpr auto& operator * ()       { return ** iterator_; }

	constexpr bool operator == (dereference_iterator i) const {
		return iterator_ == i.iterator_;
	}

	friend constexpr bool operator == (
		dereference_iterator a, Iterator b
	) { return a.iterator_ == b; }
	friend constexpr bool operator == (
		Iterator a, dereference_iterator b
	) { return a == b.iterator_; }

	constexpr auto& operator ++ () { ++iterator_; return *this; }
	constexpr auto& operator -- () { --iterator_; return *this; }
	constexpr auto& operator += (nuint n) { iterator_ += n; return *this; }
	constexpr auto& operator -= (nuint n) { iterator_ -= n; return *this; }
	constexpr auto operator + (nuint n) const {
		return dereference_iterator{ *this } += n;
	}
	constexpr nuint operator - (dereference_iterator it) const {
		return iterator_ - it.iterator_;
	}
	friend constexpr nuint operator - (dereference_iterator a, Iterator b) {
		return a.iterator_ - b;
	}
	friend constexpr nuint operator - (Iterator a, dereference_iterator b) {
		return a - b.iterator_;
	}

};

template<basic_range Range>
requires type_is_pointer<decay<range_element_type<Range>>>
class dereference_view : public range_extensions<dereference_view<Range>> {
	Range range_;

public:
	constexpr dereference_view(Range&& range) :
		range_{ forward<Range>(range) }
	{}

	constexpr auto iterator() const {
		return dereference_iterator{ range_iterator(range_) };
	}
	constexpr auto iterator()       {
		return dereference_iterator{ range_iterator(range_) };
	}

	constexpr auto sentinel() const { return range_sentinel(range_); }
	constexpr auto sentinel()       { return range_sentinel(range_); }

	constexpr auto size() const requires sized_range<Range> {
		return range_size(range_);
	}

};

template<basic_range Range>
requires type_is_pointer<decay<range_element_type<Range>>>
dereference_view(Range&&) -> dereference_view<Range>;

}