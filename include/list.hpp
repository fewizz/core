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
	StorageRange storage_range_{};
	range_iterator_type<StorageRange> storage_iterator_{};
public:

	constexpr list() = default;

	constexpr list(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) },
		storage_iterator_{ range_iterator(storage_range_) }
	{}

	constexpr list(list&& other) :
		storage_range_{ move(other.storage_range_) },
		storage_iterator_{ exchange(other.storage_iterator_, storage_iterator_type{}) }
	{}

	constexpr list& operator = (list&& other) {
		storage_range_ = move(other.storage_range_);
		storage_iterator_ = exchange(other.storage_iterator_, storage_iterator_type{});
		return *this;
	}

	/*constexpr auto& operator = (StorageRange&& storage_range) {
		clear();
		storage_range_ = move(storage_range);
		storage_iterator_ = range_iterator(storage_range_);
		return *this;
	}*/

	constexpr const StorageRange& storage_range() const {
		return storage_range_;
	}
	constexpr       StorageRange& storage_range()       {
		return storage_range_;
	}

	const_iterator_type iterator() const {
		return { storage_range_.iterator() };
	}
	iterator_type iterator() {
		return { storage_range_.iterator() };
	}

	sentinel_for<const_iterator_type> auto sentinel() const {
		return storage_iterator_;
	}
	sentinel_for<iterator_type>       auto sentinel()       {
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

	constexpr auto size() const {
		return storage_iterator_ - range_iterator(storage_range_);
	}

	constexpr auto capacity() const {
		return range_size(storage_range_);
	}

	constexpr auto available() const {
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