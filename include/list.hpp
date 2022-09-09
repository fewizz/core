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

	auto iterator() const {
		return storage_object_iterator{ storage_range_.iterator() };
	}
	auto iterator()       {
		return storage_object_iterator{ storage_range_.iterator() };
	}

	auto sentinel() const { return storage_range_.sentinel(); }
	auto sentinel()       { return storage_range_.sentinel(); }

	template<typename... Args>
	constexpr void emplace_back(Args&&... args) {
		new (storage_iterator_->data) element_type(forward<Args>(args)...);
		++storage_iterator_;
	}

	constexpr auto size() {
		return iterator_and_sentinel {
			storage_range_.iterator(),
			storage_iterator_
		}.distance();
	}

};

template<storage_range StorageRange>
list(StorageRange&&) -> list<StorageRange>;