#include <core/meta/elements/at_index.hpp>

static_assert(elements::at_index<1>(0, 1.0F) == 1.0F);

int main() {}