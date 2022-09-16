#pragma once

#include "./range.hpp"
#include "./__range/extensions.hpp"
#include "./__range/of_value_type_satisfying_predicate.hpp"

template<typename Type>
struct storage : range_extensions<storage<Type>> {
	alignas(Type) uint8 data[sizeof(Type)];

	constexpr auto iterator() const { return data; }
	constexpr auto iterator()       { return data; }

	constexpr auto sentinel() const { return data + sizeof(Type); }
	constexpr auto sentinel()       { return data + sizeof(Type); }

	template<typename... Args>
	void construct(Args&&... args) {
		new (data) Type(forward<Args>(args)...);
	}

	void destruct() {
		((Type*)data)->~Type();
	}
};

constexpr inline struct is_storage_t : type_predicate_extension<is_storage_t> {

	template<typename Type>
	struct for_non_ref_non_qualified_type_t {
		static constexpr bool value = false;
	};

	template<typename Type>
	struct for_non_ref_non_qualified_type_t<storage<Type>> {
		static constexpr bool value = true;
	};

	template<typename Type>
	constexpr bool for_type() const {
		return for_non_ref_non_qualified_type_t<
			remove_const<remove_reference<Type>>
		>::value;
	}

} is_storage{};

template<typename Type>
struct storage_element_type_t_for_non_ref_non_qualified {};

template<typename ElementType>
struct storage_element_type_t_for_non_ref_non_qualified<storage<ElementType>> {
	using type = ElementType;
};

template<typename StorageType>
requires (is_storage.template for_type<StorageType>())
using storage_element_type = typename
	storage_element_type_t_for_non_ref_non_qualified<
		remove_const<remove_reference<StorageType>>
	>::type;

template<typename StorageRange>
concept storage_range =
	range_of_value_type_satisfying_predicate<StorageRange, is_storage>;

template<basic_iterator Iterator>
class storage_range_element_iterator_base {
	Iterator iterator_;
	using storage_type = iterator_element_type<Iterator>;
	using element_type = storage_element_type<storage_type>;
public:

	storage_range_element_iterator_base(
		Iterator iterator
	) : iterator_{ iterator } {}

	decltype(auto) operator * () const {
		auto& s = *iterator_;
		return *(const element_type*) &s;
	}
	decltype(auto) operator * () {
		auto& s = *iterator_;
		return *(      element_type*) &s;
	}

	storage_range_element_iterator_base& operator ++ () {
		++iterator_;
		return *this;
	}

	bool operator == (storage_range_element_iterator_base other) const {
		return iterator_ == other.iterator_;
	}

	friend bool operator == (
		storage_range_element_iterator_base i0, Iterator i1
	) { return i0.iterator_ == i1; }

	friend bool operator == (
		Iterator i1, storage_range_element_iterator_base i0
	) { return i0.iterator_ == i1; }

	storage_range_element_iterator_base& operator += (nuint n) {
		iterator_ += n;
		return *this;
	}

	storage_range_element_iterator_base& operator -= (nuint n) {
		iterator_ -= n;
		return *this;
	}

	friend nuint operator - (
		storage_range_element_iterator_base i0,
		storage_range_element_iterator_base i1
	) { return i0.iterator_ - i1.iterator_; }

	friend storage_range_element_iterator_base operator - (
		storage_range_element_iterator_base i, nuint n
	) { return storage_range_element_iterator_base{i} -= n; }

	friend storage_range_element_iterator_base operator + (
		storage_range_element_iterator_base i, nuint n
	) { return storage_range_element_iterator_base{i} += n; }

	friend storage_range_element_iterator_base operator + (
		nuint n, storage_range_element_iterator_base i
	) { return storage_range_element_iterator_base{i} += n; }

};

template<basic_iterator Iterator>
struct storage_range_element_iterator :
	storage_range_element_iterator_base<Iterator>
{
	using base_type = storage_range_element_iterator_base<Iterator>;
	using base_type::base_type;
};

template<contiguous_iterator Iterator>
struct storage_range_element_iterator<Iterator> :
	storage_range_element_iterator_base<Iterator>,
	contiguous_iterator_mark
{
	using base_type = storage_range_element_iterator_base<Iterator>;
	using base_type::base_type;
};

template<typename Iterator>
storage_range_element_iterator(Iterator)
	-> storage_range_element_iterator<Iterator>;