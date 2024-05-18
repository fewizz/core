#pragma once

#include "./__range/extensions_implementation.hpp"  // IWYU pragma: export
#include "./__range/contiguous.hpp"  // IWYU pragma: export
#include "./__range/of_element_type_same_as.hpp"  // IWYU pragma: export
#include "./__range/of_element_type_satisfying_predicate.hpp"  // IWYU pragma: export
#include "./__range/element_type.hpp"  // IWYU pragma: export
#include "./__range/size_type.hpp"  // IWYU pragma: export
#include "./__range/growable.hpp"  // IWYU pragma: export

template<basic_range Range>
class range : public range_extensions<range<Range>> {
	Range range_;
public:

	range(Range&& range) : range_{ forward<Range>(range) } {}

	constexpr auto iterator(this auto self) {
		return range_iterator(self.range_);
	}

	constexpr auto sentinel(this auto self) {
		return range_sentinel(self.range_);
	}

	constexpr auto size() const requires sized_range<Range> {
		return range_size(range_);
	}

};

template<basic_range Range>
range(Range&&) -> range<Range>;