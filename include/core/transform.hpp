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
struct transform_view {
	Range& range;
	Function function;

	template<typename Function0>
	transform_view(Range& r, Function0&& f) :
		range{ r },
		function{ forward<Function0>(f) }
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

template<range Range, typename Function>
transform_view(Range&&, Function&&) -> transform_view<Range, Function>;

/*template<range Range>
struct transform {
	Range& range;

	transform(Range& r) : range{ r } {}

	template<typename Function>
	transform_view<Range, Function>
	with(Function&& function) const {
		return { range, forward<Function>(function) };
	}

	template<typename Function>
	transform_view<Range, Function>
	operator () (Function&& function) const {
		return with(forward<Function>(function));
	}

};*/

//template<range Range>
//transform(Range&&) -> transform<Range>;