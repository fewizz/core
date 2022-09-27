#pragma once

#include "./predicate.hpp"
#include "../range.hpp"

template<typename StorageRange>
concept storage_range =
	range_of_element_type_satisfying_predicate<StorageRange, is_storage>;