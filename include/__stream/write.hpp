#pragma once

#include "./output.hpp"
#include "./__type/is_trivial.hpp"
#include "./endianness.hpp"
#include "./integer.hpp"

namespace __stream {

	template<typename WriteType>
	struct write_t {

		template<output_stream Stream>
		requires has_inner_write<Stream, WriteType>
		constexpr void operator () (Stream&& stream, WriteType&& value) const {
			return stream.write(value);
		}

	};

}

template<typename WriteType>
inline constexpr __stream::write_t<WriteType> stream_write{};

/* template<
	trivial Type, endianness Endianness = endianness::native,
	atom_output_stream Stream
>
void write(Type value, Stream&& iterator) {
	for(nuint i = 0; i < sizeof(Type); ++i) {
		nuint i0 = Endianness != endianness::native ? sizeof(Type) - i - 1 : i;
		*iterator++ = ((uint1a*) &value)[i0];
	}
} */