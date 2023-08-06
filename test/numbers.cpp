#include <numbers.hpp>

static_assert(numbers{ 1000, 0, 1, 10, -10 }.max() == 1000);
static_assert(numbers{ 1000, 0, 1, 10, -10 }.min() == -10);

int main() {}