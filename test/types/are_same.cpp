#include <types.hpp>

static_assert(types_are_same<int, int, int, int, int>);
static_assert(types<int, int, int, int, int>::are_same);

static_assert(!types_are_same<int, int, int, int, float>);
static_assert(!types<int, int, int, int, float>::are_same);

int main(){}