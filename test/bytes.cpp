#include "../include/cxx_util/bytes.hpp"
#include <cassert>
#include <type_traits>
#include <iostream>

int main() {
	auto int_bytes = u::obj_representation<int>{ 0xFF };

	assert( u::read<int>(int_bytes) == 0xFF );

	int reverted = u::change_endianness(0xFF);
	constexpr bool big = std::endian::native == std::endian::big;
	constexpr std::endian reverted_endian
		= big ? std::endian::little : std::endian::big;

	int reverted_by_read = u::read<int, reverted_endian>(int_bytes).value();
	bool result = reverted_by_read == reverted;
	assert( result );

	u::write<int>(0x10, int_bytes);
	assert( u::obj_representation<int>{ int_bytes }.create() == 0x10 );

	u::write<int, reverted_endian>(0x10, int_bytes);
	reverted = u::change_endianness(0x10);
	assert( u::obj_representation<int>{ int_bytes }.create() == reverted );
}