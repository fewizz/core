#include <core/meta/types/are_same.hpp>

static_assert(types_are_same<int, int, int, int, int>);

static_assert(!types_are_same<int, int, int, int, float>);

int main(){}