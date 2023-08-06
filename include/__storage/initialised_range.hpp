#pragma once

#include "./range.hpp"
#include "../forward.hpp"

template<basic_range StorageRange>
requires type_is_lvalue_reference<range_element_type<StorageRange>>
struct initialised : range_extensions<initialised<StorageRange>> {
private:
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using value_type = remove_reference<range_element_type<StorageRange>>;

	StorageRange storage_range_;
public:

	initialised() = default;

	initialised(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) }
	{}

	initialised(initialised&& other) = default;
	initialised& operator = (initialised&& other) = default;

	auto iterator() const { return range_iterator(storage_range_); }
	auto iterator()       { return range_iterator(storage_range_); }

	auto sentinel() const { return range_sentinel(storage_range_); }
	auto sentinel()       { return range_sentinel(storage_range_); }

	constexpr ~initialised() {
		for(value_type& e : *this) {
			e.~value_type();
		}
	}

};