#pragma once

#include "./predicate.hpp"

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