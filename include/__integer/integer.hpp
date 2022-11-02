#pragma once

#include "atom.hpp"
#include "./if_satisfies.hpp"
#include "./__type/is_same_as.hpp"

namespace __integer {

	template<uint1a Value, uint1a Divisor>
	concept divides_without_remainder = Value % Divisor == 0;

	template<uint1a Value, uint1a Divisor>
	requires divides_without_remainder<Value, Divisor>
	inline constexpr uint1a div_without_rem = Value / Divisor;

}

template<uint1a Bits>
using int_of_bits =  int_of_atoms<
	__integer::div_without_rem<Bits, bits_per_atom>
>;

template<uint1a Bits>
using uint_of_bits = uint_of_atoms<
	__integer::div_without_rem<Bits, bits_per_atom>
>;

using  int8 =  int_of_bits<8>;
using uint8 = uint_of_bits<8>;

using  int16 =  int_of_bits<16>;
using uint16 = uint_of_bits<16>;

using  int32 =  int_of_bits<32>;
using uint32 = uint_of_bits<32>;

using  int64 =  int_of_bits<64>;
using uint64 = uint_of_bits<64>;