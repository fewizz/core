#include "../include/cxx_util/bytes.hpp"
#include <cassert>

int main() {
	auto int_bytes = u::obj_representation<int>{ 0xFF };

	assert( u::read<std::byte>(int_bytes) == std::byte{ 0xFF } );
}