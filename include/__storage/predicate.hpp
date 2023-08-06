#pragma once

#include "../__type/predicate.hpp"
#include "./storage.hpp"

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

constexpr inline struct is_storage_of_specified_size_and_alignment_t
	: type_predicate_extension<is_storage_of_specified_size_and_alignment_t>
{

	template<typename Type>
	struct for_non_ref_non_qualified_type_t {
		static constexpr bool value = false;
	};

	template<nuint Size, nuint Alignment>
	struct for_non_ref_non_qualified_type_t<
		storage_of_size_and_alignment<Size, Alignment>
	> {
		static constexpr bool value = true;
	};

	template<typename Type>
	constexpr bool for_type() const {
		return for_non_ref_non_qualified_type_t<
			remove_const<remove_reference<Type>>
		>::value;
	}

} is_storage_of_specified_size_and_alignment{};