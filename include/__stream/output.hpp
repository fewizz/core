#pragma once

#include "../type.hpp"
#include "../integer.hpp"

struct output_stream_mark {};

template<typename Type>
concept output_stream = base_of<
	remove_const<remove_reference<Type>>,
	output_stream_mark
>;

namespace __stream {

	template<typename Type, typename WriteType>
	concept has_inner_write =
		requires(Type stream, WriteType t) {
			stream.write(t);
		};

	template<typename Type, typename WriteType>
	concept writable =
		has_inner_write<Type, WriteType>;

}

template<typename Type>
concept atom_output_stream =
	output_stream<Type> &&
	__stream::writable<Type, uint1a>;