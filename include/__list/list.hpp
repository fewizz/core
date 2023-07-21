#pragma once

#include "./declaration.hpp"
#include "../__storage/range.hpp"
#include "../__range/growable.hpp"
#include "../__range/borrowed.hpp"

template<storage_range StorageRange>
requires (!borrowed_range<StorageRange>)
class list : public range_extensions<list<StorageRange>> {
	using storage_iterator_type = range_iterator_type<StorageRange>;
	using storage_const_iterator_type = range_iterator_type<const StorageRange>;
	using storage_type = remove_reference<remove_const<
		range_element_type<StorageRange>
	>>;
	using element_type = typename storage_type::type;

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
	constexpr list(nuint n, list&& other) :
		storage_range_{ move(other.storage_range_) },
		sentinel_ { range_iterator(storage_range_) + n }
	{
		other.sentinel_ = range_iterator(other.storage_range_);
	}
public:

	constexpr list& operator = (list&& other) {
		clear();
		nuint n = other.size();
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

	auto iterator() const {
		if constexpr(contiguous_range<StorageRange>) { // TODO
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
			return (const element_type*) sentinel_;
		}
		else {
			return sentinel_;
		}
	}
	auto sentinel()       {
		if constexpr(contiguous_range<StorageRange>) {
			return (element_type*) sentinel_;
		}
		else {
			return sentinel_;
		}
	}

	template<typename... Args>
	constexpr element_type& emplace_back(Args&&... args) {
		if constexpr(growable_range<StorageRange>) {
			if(size() == capacity()) {
				storage_range_.grow();
				sentinel_ = storage_range_.iterator() + size();
			}
		}
		storage_type& s = (*(sentinel_++));
		return s.construct(forward<Args>(args)...);
	}

	constexpr void ensure_size(nuint size) /* requires default_init */ {
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
		storage_type& s = *(range_iterator(storage_range_) + index);
		s.destruct();
		s.template construct<Args...>(forward<Args>(args)...);
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

	element_type pop_back() requires move_constructible<element_type> {
		--sentinel_;
		storage_type& s = (*sentinel_);
		element_type e(s.move());
		s.destruct();
		return e;
	}

	constexpr void erase_back() {
		--sentinel_;
		(*sentinel_).destruct();
	}

	constexpr void erase_back(nuint n) {
		while(n > 0) {
			erase_back();
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
			erase_back();
		}
	}

};

template<storage_range StorageRange>
list(StorageRange&&) -> list<StorageRange>;