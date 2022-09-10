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