#pragma once

#include "./storage.hpp"
#include "./exchange.hpp"
#include "./placement_new.hpp"
#include "./iterator_and_sentinel.hpp"
#include "./__range/extensions.hpp"
#include "./__type/is_move_constructible.hpp"
#include "./__iterator/sentinel.hpp"

template<basic_range StorageRange>
class list;

template<
	storage_range StorageRange
>
class list<StorageRange> : public range_extensions<list<StorageRange>> {
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using storage_type = remove_reference<remove_const<
		range_element_type<StorageRange>
	>>;
	using value_type = storage_type::type;

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
		return e;
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
	Type&& pop_back() requires move_constructible<Type> {
		--sentinel_;
		storage_type s = (*sentinel_);
		Type&& e = s.template move<Type>();
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

template<storage_range StorageRange>
list(StorageRange&&) -> list<StorageRange>;