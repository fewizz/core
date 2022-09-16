#pragma once

#include "./predicate.hpp"
#include "../range.hpp"

template<typename StorageRange>
concept storage_range =
	range_of_value_type_satisfying_predicate<StorageRange, is_storage>;