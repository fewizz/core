#include <integer.hpp>
#include <type.hpp>

static_assert(same_as<uint_for_states_count<1>, uint8>);
static_assert(same_as<uint_for_states_count<256>, uint8>);
static_assert(same_as<uint_for_states_count<257>, uint16>);
static_assert(same_as<uint_for_states_count<65536>, uint16>);
static_assert(same_as<uint_for_states_count<65537>, uint32>);
static_assert(same_as<uint_for_states_count<uint32{0} - uint32{1}>, uint32>);
static_assert(same_as<uint_for_states_count<(uint64{ 1 } << 32)>, uint32>);
static_assert(same_as<uint_for_states_count<(uint64{ 1 } << 32) + 1>, uint64>);
static_assert(same_as<uint_for_states_count<uint64{0} - uint64{1}>, uint64>);

int main() {}