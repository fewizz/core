#pragma once

#include "./storage.hpp"
#include "./placement_new.hpp"
#include "./iterator_and_sentinel.hpp"
#include "./__range/extensions.hpp"

template<
	storage_range StorageRange
>
class list : public range_extensions<list<StorageRange>> {
	using element_type = storage_element_type<range_element_type<StorageRange>>;
	StorageRange storage_range_;
	range_iterator_type<StorageRange> storage_iterator_;
public:

	constexpr list(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) },
		storage_iterator_{ storage_range_.iterator() }
	{}

	const range_iterator_type<StorageRange> iterator() const {
		return storage_range_.iterator();
	}
	      range_iterator_type<StorageRange> iterator()       {
		return storage_range_.iterator();
	}

	const range_sentinel_type<StorageRange> sentinel() const {
		return storage_iterator_;
	}
	      range_sentinel_type<StorageRange> sentinel()       {
		return storage_iterator_;
	}

	template<typename... Args>
	constexpr void emplace_back(Args&&... args) {
		new (storage_iterator_->data) element_type(forward<Args>(args)...);
		++storage_iterator_;
	}

	constexpr range_size_type<StorageRange> capacity() const {
		return range_size(storage_range_);
	}

	constexpr void fill(auto&& something) {
		while(this->size() < capacity()) {
			emplace_back(something);
		}
	}

};

template<storage_range StorageRange>
list(StorageRange&&) -> list<StorageRange>;