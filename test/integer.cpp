#include "integer.hpp"
#include "primitive_integer.hpp"

static_assert(are_same<signed_integer_of_bits<32>::type, primitive::int32>);
static_assert(sizeof(signed_integer_of_bits<32>) == 4);

int main() {
}