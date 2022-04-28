#pragma once

#include <core/value_type.hpp>
#include <core/meta/type/is_same_as.hpp>

static_assert(same_as<value_type<int[10]>, int>);

int main() {}