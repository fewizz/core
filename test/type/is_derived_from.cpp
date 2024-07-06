#include <__type/is_derived_from.hpp>

struct a{};
struct b : a{};

static_assert(derived_from<b, a>);
static_assert(is_derived_from<a>.for_type<b>());