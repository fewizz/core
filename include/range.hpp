#pragma once

#include "./__range/extensions_implementation.hpp"
#include "./__range/contiguous.hpp"
#include "./__range/of_element_type_same_as.hpp"
#include "./__range/of_element_type_satisfying_predicate.hpp"
#include "./__range/element_type.hpp"
#include "./__range/size_type.hpp"
#include "./__range/growable.hpp"

template<basic_range Range>
class range : public range_extensions<range<Range>> {
	Range range_;
public:

	range(Range&& range) : range_{ forward<Range>(range) } {}

	constexpr auto iterator() const { return range_iterator(range_); }
	constexpr auto iterator()       { return range_iterator(range_); }

	constexpr auto sentinel() const { return range_sentinel(range_); }
	constexpr auto sentinel()       { return range_sentinel(range_); }

};

template<basic_range Range>
range(Range&&) -> range<Range>;