#pragma once

#include "./range.hpp"
#include "../forward.hpp"

template<storage_range StorageRange>
struct initialised : range_extensions<initialised<StorageRange>> {
private:
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using storage_type = remove_reference<remove_const<
		range_element_type<StorageRange>
	>>;
	using element_type = typename storage_type::type;

	StorageRange storage_range_;
public:

	initialised() = default;

	initialised(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) }
	{}

	initialised(initialised&& other) = default;
	initialised& operator = (initialised&& other) = default;

	auto iterator() const {
		if constexpr(contiguous_range<StorageRange>) {
			return (const element_type*) storage_range_.iterator();
		}
		else {
			return range{ storage_range_ }
				.transform_view([](auto& storage) -> auto& {
					return storage.get();
				})
				.iterator();
		}
	}
	auto iterator()       {
		if constexpr(contiguous_range<StorageRange>) {
			return (element_type*) storage_range_.iterator();
		}
		else {
			return range{ storage_range_ }
				.transform_view([](auto& storage) -> auto& {
					return storage.get();
				})
				.iterator();
		}
	}

	auto sentinel() const {
		if constexpr(contiguous_range<StorageRange>) {
			return (element_type*) storage_range_.sentinel();
		}
		else {
			return range{ storage_range_ }
				.transform_view([](auto& storage) -> auto& {
					return storage.get();
				})
				.sentinel();
		}
	}
	auto sentinel() {
		if constexpr(contiguous_range<StorageRange>) {
			return (element_type*) storage_range_.sentinel();
		}
		else {
			return range{ storage_range_ }
				.transform_view([](auto& storage) -> auto& {
					return storage.get();
				})
				.sentinel();
		}
	}

	constexpr auto size() const {
		return range_size(storage_range_);
	}

	constexpr ~initialised() {
		for(decltype(auto) e : *this) {
			e.~element_type();
		}
	}

};