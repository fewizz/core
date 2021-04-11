#include "mem_address.hpp"
#include <cassert>
#include <iterator>

static_assert(std::input_iterator<u::mem_address>);
static_assert(std::output_iterator<u::mem_address, std::byte>);
static_assert(std::forward_iterator<u::mem_address>);
static_assert(std::bidirectional_iterator<u::mem_address>);
static_assert(std::random_access_iterator<u::mem_address>);
static_assert(std::contiguous_iterator<u::mem_address>);

int main() {
	int val = 0xFF;
	
	u::mem_address mi { &val };

	assert( *mi == std::byte{ 0xFF } );
	assert( *(mi + 1) == std::byte{ 0x00 } );
	assert( (&val + 1) == (mi + sizeof(int)) );
}