#pragma once

#include "./storage.hpp"
#include "./element_type.hpp"
#include "../__iterator/element_type.hpp"
#include "../__iterator/contiguous.hpp"

template<basic_iterator Iterator>
struct storage_range_element_iterator;

template<basic_iterator Iterator>
class storage_range_element_iterator_base {
	Iterator iterator_;
	using Derived = storage_range_element_iterator<Iterator>;
	using storage_type = iterator_element_type<Iterator>;
	using element_type = storage_element_type<storage_type>;

	const Derived& derived() const { return (const Derived&) *this; }
	      Derived& derived()       { return (      Derived&) *this; }

public:

	storage_range_element_iterator_base(
		Iterator iterator
	) : iterator_{ iterator } {}

	decltype(auto) operator * () const {
		auto& s = *iterator_;
		return *(const element_type*) &s;
	}
	decltype(auto) operator * () {
		auto& s = *iterator_;
		return *(      element_type*) &s;
	}

	Derived& operator ++ () { ++iterator_; return derived(); }
	Derived& operator -- () { --iterator_; return derived(); }

	bool operator == (storage_range_element_iterator_base other) const {
		return iterator_ == other.iterator_;
	}

	friend bool operator == (
		storage_range_element_iterator_base i0, Iterator i1
	) { return i0.iterator_ == i1; }

	friend bool operator == (
		Iterator i1, storage_range_element_iterator_base i0
	) { return i0.iterator_ == i1; }

	Derived& operator += (nuint n) { iterator_ += n; return derived(); }
	Derived& operator -= (nuint n) { iterator_ -= n; return derived(); }

	friend nuint operator - (
		storage_range_element_iterator_base i0,
		storage_range_element_iterator_base i1
	) { return i0.iterator_ - i1.iterator_; }

	friend storage_range_element_iterator_base operator - (
		storage_range_element_iterator_base i, nuint n
	) { return storage_range_element_iterator_base{i} -= n; }

	friend Derived operator + (Derived i, nuint n) {
		return Derived{ i } += n;
	}

	friend Derived operator + (nuint n, Derived i) {
		return Derived{i} += n;
	}

};

template<basic_iterator Iterator>
struct storage_range_element_iterator :
	storage_range_element_iterator_base<Iterator>
{
	using base_type = storage_range_element_iterator_base<Iterator>;
	using base_type::base_type;
};

template<contiguous_iterator Iterator>
struct storage_range_element_iterator<Iterator> :
	storage_range_element_iterator_base<Iterator>,
	contiguous_iterator_mark
{
	using base_type = storage_range_element_iterator_base<Iterator>;
	using base_type::base_type;
};

template<typename Iterator>
storage_range_element_iterator(Iterator)
	-> storage_range_element_iterator<Iterator>;