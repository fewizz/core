#pragma once

#include "./output.hpp"
#include "../__type/is_trivial.hpp"
#include "../endianness.hpp"
#include "../integer.hpp"

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