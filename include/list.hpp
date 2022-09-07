#pragma once

#include "./range.hpp"
#include "./storage.hpp"
#include "./placement_new.hpp"

template<
	typename ElementType,
	range_of<storage<ElementType>> StorageRange,
	typename SizeType = nuint
>
class list {
	range_iterator_type<StorageRange> storage_iterator_;
public:

	list() = default;

	template<typename... Args>
	void emplace_back(Args&&... args) {
		new (*storage_iterator_.data) ElementType(forward<Args>(args)...);
		++storage_iterator_;
	}

	

};