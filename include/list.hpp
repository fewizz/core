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
	using size_type = range_size_type<StorageRange>;
	using iterator_type = range_iterator_type<StorageRange>;
	using const_iterator_type = range_iterator_type<const StorageRange>;
	StorageRange storage_range_{};
	range_iterator_type<StorageRange> storage_iterator_;
public:

	constexpr list() : storage_iterator_{ storage_range_.iterator() } {}

	constexpr list(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) },
		storage_iterator_{ storage_range_.iterator() }
	{}

	storage_range_element_iterator<const_iterator_type> iterator() const {
		return storage_range_element_iterator{ storage_range_.iterator() };
	}
	storage_range_element_iterator<      iterator_type> iterator() {
		return storage_range_element_iterator{ storage_range_.iterator() };
	}

	range_sentinel_type<const StorageRange> sentinel() const {
		return storage_iterator_;
	}
	range_sentinel_type<StorageRange>       sentinel()       {
		return storage_iterator_;
	}

	template<typename... Args>
	constexpr void emplace_back(Args&&... args) {
		(*storage_iterator_).construct(forward<Args>(args)...);
		++storage_iterator_;
	}

	constexpr void pop_back() {
		--storage_iterator_;
		(*storage_iterator_).destruct();
	}

	constexpr size_type capacity() const {
		return range_size(storage_range_);
	}

	constexpr size_type available() const {
		return capacity() - this->size();
	}

	constexpr void fill_with(auto&& something) {
		while(this->size() < capacity()) {
			emplace_back(something);
		}
	}

	template<basic_range Range>
	constexpr void put_back_copied_elements_of(Range&& other) {
		for(decltype(auto) e : other) {
			emplace_back(forward<decltype(e)>(e));
		}
	}

	void clear() {
		while(this->size() > 0) {
			pop_back();
		}
	}

};

template<storage_range StorageRange>
list(StorageRange&&) -> list<StorageRange>;