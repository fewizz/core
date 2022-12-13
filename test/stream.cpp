#include <__stream/output.hpp>
#include <__stream/write.hpp>
#include <expression_of_type.hpp>

struct ostream_with_inner_write : output_stream_mark {

	void write([[maybe_unused]] uint1a v) {
		return;
	}

};

static_assert(output_stream<ostream_with_inner_write>);

static_assert(
same_as<
	decltype(
		expression_of_type<ostream_with_inner_write>.write(
			uint1a{ 0 }
		)
	),
	void
>);

static_assert(
same_as<
	decltype(
		stream_write<uint1a>(
			expression_of_type<ostream_with_inner_write>,
			uint1a{ 0 }
		)
	),
	void
>);