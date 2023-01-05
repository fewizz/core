#pragma once

#include "./storage.hpp"
#include "./exchange.hpp"
#include "./placement_new.hpp"
#include "./iterator_and_sentinel.hpp"
#include "./__range/extensions.hpp"
#include "./__type/is_move_constructible.hpp"
#include "./__iterator/sentinel.hpp"

template<
	storage_range StorageRange
>
class list : public range_extensions<list<StorageRange>> {
	using element_type = storage_element_type<range_element_type<StorageRange>>;
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using iterator_type = storage_range_element_iterator<storage_iterator_type>;
	using const_iterator_type = storage_range_element_iterator<
		storage_const_iterator_type
	>;
	StorageRange storage_range_;
	storage_iterator_type storage_iterator_;
public:

	constexpr ~list() {
		//clear();
	}

	constexpr list() :
		storage_range_{}, storage_iterator_{ range_iterator(storage_range_) }
	{}

	constexpr list(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) },
		storage_iterator_{ range_iterator(storage_range_) }
	{}

	constexpr list(list&& other) :
		storage_range_{ move(other.storage_range_) },
		storage_iterator_{ exchange(
				other.storage_iterator_,
				range_iterator(other.storage_range_)
		)}
	{}

	constexpr list& operator = (list&& other) {
		clear();
		storage_range_ = move(other.storage_range_);
		storage_iterator_ = exchange(
			other.storage_iterator_, range_iterator(other.storage_range_)
		);
		return *this;
	}

	constexpr list& operator = (StorageRange&& storage_range) {
		clear();
		storage_range_ = move(storage_range);
		storage_iterator_ = range_iterator(storage_range_);
		return *this;
	}

	constexpr remove_reference<StorageRange>& storage_range() & {
		return this->storage_range_;
	}

	constexpr StorageRange move_storage_range()
	requires (!type_is_reference<StorageRange>) {
		StorageRange moved = move(this->storage_range_);
		storage_iterator_ = range_iterator(this->storage_range_);
		return move(moved);
	}

	const_iterator_type iterator() const {
		return { range_iterator(storage_range_) };
	}
	      iterator_type iterator()       {
		return { range_iterator(storage_range_) };
	}

	sentinel_for<const_iterator_type> auto sentinel() const {
		return storage_iterator_;
	}
	sentinel_for<      iterator_type> auto sentinel()       {
		return storage_iterator_;
	}

	template<typename... Args>
	constexpr element_type& emplace_back(Args&&... args) {
		return (*(storage_iterator_++)).construct(forward<Args>(args)...);
	}

	template<typename... Args>
	constexpr void emplace_at(auto index, Args&&... args) {
		auto& s = *(range_iterator(storage_range_) + index);
		s.destruct();
		s.construct(forward<Args>(args)...);
	}

	constexpr element_type
	pop_back() requires move_constructible<element_type> {
		--storage_iterator_;
		auto&& e = (*storage_iterator_).move();
		(*storage_iterator_).destruct();
		return e;
	}
	constexpr void
	pop_back() requires (!move_constructible<element_type>) {
		--storage_iterator_;
		(*storage_iterator_).destruct();
	}

	constexpr void pop_back(nuint n) {
		while(n > 0) {
			pop_back();
			--n;
		}
	}

	auto& back() const & { return (*(storage_iterator_ - 1)).get(); }
	auto& back()       & { return (*(storage_iterator_ - 1)).get(); }

	constexpr nuint size() const {
		return storage_iterator_ - range_iterator(storage_range_);
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
		for(auto& e : other) {
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