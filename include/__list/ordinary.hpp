#pragma once

#include "./declaration.hpp"
#include "../__storage/range.hpp"
#include "../__range/growable.hpp"

template<
	storage_range StorageRange
>
class list<StorageRange> : public range_extensions<list<StorageRange>> {
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using storage_type = remove_reference<remove_const<
		range_element_type<StorageRange>
	>>;
	using value_type = typename storage_type::type;

	StorageRange storage_range_;
	storage_iterator_type sentinel_;
public:

	constexpr list() :
		storage_range_{}, sentinel_{ range_iterator(storage_range_) }
	{}

	constexpr list(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) },
		sentinel_{ range_iterator(storage_range_) }
	{}

	constexpr list(list&& other) :
		storage_range_{ move(other.storage_range_) },
		sentinel_ {
			exchange(
				other.storage_iterator_,
				range_iterator(other.storage_range_)
			)
		}
	{}

	constexpr list& operator = (list&& other) {
		storage_range_ = move(other.storage_range_);
		sentinel_ = exchange(
			other.storage_iterator_, range_iterator(other.storage_range_)
		);
		return *this;
	}

	constexpr list& operator = (StorageRange&& storage_range) {
		storage_range_ = move(storage_range);
		sentinel_ = range_iterator(storage_range_);
		return *this;
	}

	constexpr remove_reference<StorageRange>& storage_range() & {
		return this->storage_range_;
	}

	auto iterator() const {
		if constexpr(contiguous_range<StorageRange>) { // TODO
			return (const value_type*) storage_range_.iterator();
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
			return (value_type*) storage_range_.iterator();
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
			return (const value_type*) sentinel_;
		}
		else {
			return sentinel_;
		}
	}
	auto sentinel()       {
		if constexpr(contiguous_range<StorageRange>) {
			return (value_type*) sentinel_;
		}
		else {
			return sentinel_;
		}
	}

	template<typename... Args>
	constexpr value_type& emplace_back(Args&&... args) {
		if constexpr(growable_range<StorageRange>) {
			if(size() == capacity()) {
				storage_range_.grow();
				sentinel_ = storage_range_.iterator() + size();
			}
		}
		storage_type& s = (*(sentinel_++));
		return s.construct(forward<Args>(args)...);
	}

	template<typename... Args>
	constexpr void emplace_at(auto index, Args&&... args) {
		storage_type& s = *(range_iterator(storage_range_) + index);
		s.destruct();
		s.template construct<Args...>(forward<Args>(args)...);
	}

	value_type&& pop_back() requires move_constructible<value_type> {
		--sentinel_;
		storage_type s = (*sentinel_);
		value_type&& e = s.move();
		s.destruct();
		return move(e);
	}
	constexpr void
	pop_back() requires (!move_constructible<value_type>) {
		--sentinel_;
		(*sentinel_).destruct();
	}

	constexpr void pop_back(nuint n) {
		while(n > 0) {
			pop_back();
			--n;
		}
	}

	auto& back() const & { return (*(sentinel_ - 1)).get(); }
	auto& back()       & { return (*(sentinel_ - 1)).get(); }

	constexpr nuint size() const {
		return sentinel_ - range_iterator(storage_range_);
	}

	constexpr nuint capacity() const {
		return range_size(storage_range_);
	}

	constexpr nuint available() const {
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
			emplace_back(e);
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