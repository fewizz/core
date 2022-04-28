#pragma once

#include <core/element_type.hpp>
#include <core/meta/type/is_same_as.hpp>

static_assert(same_as<element_type<int[10]>, int&>);

int main() {}