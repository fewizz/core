#pragma once

#include "range.hpp"
#include "forward.hpp"
#include "meta/type/decay.hpp"
#include "value_type.hpp"

template<typename Iterator, typename Function>
struct transformed_iterator {
	using base_iterator_type = Iterator;

	Iterator iterator;
	Function& function;

	constexpr transformed_iterator(Iterator iterator, Function& function) :
		iterator{ iterator },
		function{ function }
	{}

	constexpr decltype(auto) operator * () const {
		return function(*iterator);
	}

	constexpr decltype(auto) operator * () {
		return function(*iterator);
	}

	constexpr auto& operator ++ () {
		++iterator;
		return *this;
	}

	constexpr bool operator == (const transformed_iterator& other) const {
		return iterator == other.iterator;
	}

};

template<range Range, typename Function>
struct transformed {
	Range& range;
	Function&& function;

	transformed(Range& r, Function&& f) :
		range{ r },
		function{ forward<Function>(f) }
	{}

	auto begin() {
		return transformed_iterator {
			range.begin(),
			function
		};
	}

	auto begin() const {
		return transformed_iterator {
			range.begin(),
			function
		};
	}

	auto end() {
		return transformed_iterator {
			range.end(),
			function
		};
	}

	auto end() const {
		return transformed_iterator {
			range.end(),
			function
		};
	}
};

template<range Range>
struct transform {
	Range& range;

	transform(Range& r) : range{ r } {}

	template<typename F>
	transformed<Range, F> operator () (F&& f) const {
		return { range, forward<F>(f) };
	}

};

template<range Range>
transform(Range&&) -> transform<Range>;