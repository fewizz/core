#pragma once

#include "./declaration.hpp"
#include "../__storage/range.hpp"
#include "../__range/growable.hpp"
#include "../__range/borrowed.hpp"
#include "../__storage/initialised_range.hpp"
#include "../on_scope_exit.hpp"

template<basic_range StorageRange>
requires (
	!borrowed_range<StorageRange> &&
	type_is_lvalue_reference<range_element_type<StorageRange>>
)
class list : public range_extensions<list<StorageRange>> {
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using value_type = remove_reference<range_element_type<StorageRange>>;
	using size_type = range_size_type<StorageRange>;

	StorageRange storage_range_;
	storage_iterator_type sentinel_;
public:

	constexpr ~list() {
		clear();
	}

	constexpr list() :
		storage_range_{}, sentinel_{ range_iterator(storage_range_) }
	{}

	constexpr list(StorageRange&& storage_range) :
		storage_range_{ forward<StorageRange>(storage_range) },
		sentinel_{ range_iterator(storage_range_) }
	{}

	constexpr list(list&& other) : list(other.size(), move(other)) {}

private:
	constexpr list(size_type n, list&& other) :
		storage_range_{ move(other.storage_range_) },
		sentinel_ { range_iterator(storage_range_) + n }
	{
		other.sentinel_ = range_iterator(other.storage_range_);
	}
public:

	constexpr list& operator = (list&& other) {
		clear();
		size_type n = other.size();
		storage_range_ = move(other.storage_range_);
		sentinel_ = range_iterator(storage_range_) + n;
		other.sentinel_ = range_iterator(other.storage_range_);
		return *this;
	}

	constexpr StorageRange move_storage_range() {
		StorageRange storage = move(storage_range_);
		sentinel_ = range_iterator(storage_range_);
		return storage;
	}

	auto iterator() const { return range_iterator(storage_range_); }
	auto iterator()       { return range_iterator(storage_range_); }

	auto sentinel() const { return sentinel_; }
	auto sentinel()       { return sentinel_; }

	template<typename... Args>
	constexpr value_type& emplace_back(Args&&... args) {
		if constexpr(growable_range<StorageRange>) {
			if(size() == capacity()) {
				storage_range_.grow();
				sentinel_ = storage_range_.iterator() + size();
			}
		}
		value_type& e = *(sentinel_++);
		return *new (&e) value_type(forward<Args>(args)...);
	}

	template<typename... Args>
	constexpr size_type emplace_back_and_get_index(Args&&... args) {
		size_type index = size();
		emplace_back(forward<Args>(args)...);
		return index;
	}

	constexpr void ensure_size(size_type size) /* requires default_init */ {
		while(size > this->size()) {
			emplace_back();
		}
	}

	template<basic_range OtherRange>
	constexpr void emplace_back_elements_of(OtherRange&& other) {
		for(decltype(auto) e : forward<OtherRange>(other)) {
			emplace_back(forward<decltype(e)>(e));
		}
	}

	template<typename... Args>
	constexpr void emplace_at(auto index, Args&&... args) {
		value_type& e = *(range_iterator(storage_range_) + index);
		e.~value_type();
		new (&e) value_type(forward<Args>(args)...);
	}

	struct output_stream_t {
		list& list_;

		template<typename Type>
		void write(Type&& t) {
			list_.emplace_back(forward<Type>(t));
		}
	};

	constexpr output_stream_t output_stream() {
		return output_stream_t{ *this };
	}

	value_type pop_back() requires move_constructible<value_type> {
		value_type moved = move(back());
		erase_back();
		return moved;
	}

	constexpr void erase_back() {
		--sentinel_;
		(*sentinel_).~value_type();
	}

	constexpr void erase_back(size_type n) {
		while(n > 0) {
			erase_back();
			--n;
		}
	}

	auto& back() const & { return *(sentinel_ - 1); }
	auto& back()       & { return *(sentinel_ - 1); }

	constexpr size_type size() const {
		return sentinel_ - range_iterator(storage_range_);
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
			emplace_back(e);
		}
	}

	void clear() {
		while(this->size() > 0) {
			erase_back();
		}
	}

	initialised<StorageRange> as_initialised() && {
		on_scope_exit reset_sentinel {
			[&]{ sentinel_ = range_iterator(storage_range_); }
		};
		return { forward<StorageRange>(storage_range_) };
	}

};

template<storage_range StorageRange>
list(StorageRange&&) -> list<StorageRange>;