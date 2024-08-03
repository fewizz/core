#pragma once

#include "./predicate.hpp"
#include "../range.hpp"

template<typename StorageRange>
concept storage_range =
	range_of<StorageRange, is_storage>;

template<typename StorageRange>
concept range_of_storage_of_specified_size_and_alignment =
	range_of<
		StorageRange, is_storage_of_specified_size_and_alignment
	>;