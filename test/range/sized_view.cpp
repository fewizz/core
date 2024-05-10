#include <range.hpp>
#include <ranges.hpp>

#include <c_string.hpp>

static_assert(
	c_string{ "A" }.sized().size() == 1
);
static_assert(
	c_string{ "BCDEF" }.sized_view().size() == 5
);

static_assert(
	ranges {
		c_string{ "A" }.sized(),
		c_string{ "BCDEF" }.sized()
	}.concat_view().sized_view().size() == 6
);

int main() {}