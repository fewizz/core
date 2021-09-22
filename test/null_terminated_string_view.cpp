#include "null_terminated_string_view.hpp"
#include "range.hpp"

static_assert(range<null_terminated_string_view<size_is::defined>>);
static_assert(range<null_terminated_string_view<size_is::undefined>>);

constexpr const char* str = "";
using from_ccp = decltype(null_terminated_string_view{str});
static_assert(are_same<from_ccp, null_terminated_string_view<size_is::undefined>>);

using from_array = decltype(null_terminated_string_view{""});
static_assert(are_same<from_array, null_terminated_string_view<size_is::defined>>);

int main() {
	static_assert(null_terminated_string_view{"asdasd"}.size() == 6u);
}