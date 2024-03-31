#include <__type/is_same_as.hpp>
#include <__type/copy_const_ref.hpp>

static_assert(same_as<copy_const_ref<int, float>, float>);
static_assert(same_as<copy_const_ref<int&, float>, float&>);
static_assert(same_as<copy_const_ref<int&&, float>, float&&>);

static_assert(same_as<copy_const_ref<const int, float>, const float>);
static_assert(same_as<copy_const_ref<const int&, float>, const float&>);
static_assert(same_as<copy_const_ref<const int&&, float>, const float&&>);