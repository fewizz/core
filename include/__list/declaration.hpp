#pragma once

#include "../__range/basic.hpp"
#include "../__range/borrowed.hpp"
#include "../__storage/range.hpp"

template<storage_range StorageRange>
requires (!borrowed_range<StorageRange>)
class list;