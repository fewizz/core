#pragma once

#include "./basic.hpp"
#include "./extensions.hpp"
#include "./iterator.hpp"
#include "./element_index_type.hpp"
#include "../forward.hpp"
#include "../tuple.hpp"

namespace __range {


template<basic_range Range>
class indexed_view : public range_extensions<indexed_view<Range>> {
	Range range_;

	class indexed_iterator {
		using iterator_t = range_iterator_type<Range>;
		using sentinel_t = range_sentinel_type<Range>;
		using element_t = range_element_type<Range>;
		using index_t = range_element_index_type<Range>;

		index_t index_;
		iterator_t iterator_;
	public:

		constexpr indexed_iterator(index_t index, iterator_t iterator):
			index_{index}, iterator_{iterator}
		{}

		constexpr tuple<index_t, element_t> operator * () {
			return {index_, *iterator_};
		}

		constexpr auto& operator ++ () {
			++iterator_; ++index_;
			return *this;
		}

		friend constexpr bool operator == (indexed_iterator self, sentinel_t sentinel) {
			return self.iterator_ == sentinel;
		}
		friend constexpr bool operator == (sentinel_t sentinel, indexed_iterator self) {
			return self.iterator_ == sentinel;
		}
	};

public:

	constexpr indexed_view(Range&& range): range_{forward<Range>(range)} {}

	constexpr indexed_iterator iterator() {
		return {
			range_element_index_type<Range>{},
			range_.iterator()
		};
	}

	constexpr auto sentinel() { return range_.sentinel(); }

};

template<basic_range Range>
indexed_view(Range&&) -> indexed_view<Range>;


} // __range