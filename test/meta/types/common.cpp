#include <core/meta/types/common.hpp>

using namespace types;

static_assert(
	same_as<common::for_types<int, int&, int&>, int>
);