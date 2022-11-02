#pragma once

#include "./integer.hpp"

// TODO redo using atoms
template<uint64 StatesCount>
/* returns
   uint8 for  0   < `StatesCount`<= 256,
   uint16 for 256 < `StatesCount`<= 65536,
   uint32 for 65536 < `StatesCount`< max unsigned int,
   uint64 otherwise.
*/
using uint_for_states_count = typename
	if_satisfies<(StatesCount <= (uint64{ 1 } << 8))>::template
	then<uint8>::template
	otherwise<
		typename if_satisfies<(StatesCount <= (uint64{ 1 } << 16))>::template
		then<uint16>::template
		otherwise<
			typename if_satisfies<(StatesCount <= (uint64{ 1 } << 32))>::template
			then<uint32>::template
			otherwise<
				uint64
			>
		>
	>;