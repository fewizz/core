#pragma once

#include "./declaration.hpp"
#include "../__storage/range.hpp"

template<
	range_of_storage_of_specified_size_and_alignment StorageRange
>
class list<StorageRange> : public range_extensions<list<StorageRange>> {
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using storage_type = remove_reference<
		remove_const<range_element_type<StorageRange>>
	>;

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
				other.sentinel_,
				range_iterator(other.storage_range_)
			)
		}
	{}

	constexpr list& operator = (list&& other) {
		storage_range_ = move(other.storage_range_);
		sentinel_ = exchange(
			other.sentinel_, range_iterator(other.storage_range_)
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

	storage_const_iterator_type iterator() const {
		return { range_iterator(storage_range_) };
	}
	      storage_iterator_type iterator()       {
		return { range_iterator(storage_range_) };
	}

	sentinel_for<storage_const_iterator_type> auto sentinel() const {
		return sentinel_;
	}
	sentinel_for<      storage_iterator_type> auto sentinel()       {
		return sentinel_;
	}

	template<typename Type, typename... Args>
	constexpr Type& emplace_back(Args&&... args) {
		storage_type& s = (*(sentinel_++));
		return s.template construct<Type>(forward<Args>(args)...);
	}

	template<typename Type, typename... Args>
	constexpr void emplace_at(auto index, Args&&... args) {
		storage_type& s = *(range_iterator(storage_range_) + index);
		s.template destruct<Type>();
		s.template construct<Type, Args...>(forward<Args>(args)...);
	}

	template<typename Type>
	Type pop_back() requires move_constructible<Type> {
		--sentinel_;
		storage_type& s = (*sentinel_);
		Type e = s.template move<Type>();
		s.template destruct<Type>();
		return e;
	}
	template<typename Type>
	constexpr void
	pop_back() requires (!move_constructible<Type>) {
		--sentinel_;
		(*sentinel_).template destruct<Type>();
	}

	template<typename Type>
	constexpr void pop_back(nuint n) {
		while(n > 0) {
			pop_back<Type>();
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
			emplace_back<
				remove_reference<remove_const<range_element_type<Range>>>
			>(e);
		}
	}

	template<typename Type>
	void clear() {
		while(this->size() > 0) {
			pop_back<Type>();
		}
	}

};