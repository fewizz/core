#include "meta/type_at.hpp"
#include "meta/are_types_same.hpp"

static_assert(are_types_same<
	type_at<0u>::template for_types_of<int, float, bool>,
	int
>);

static_assert(are_types_same<
	type_at<1u>::template for_types_of<int, float, bool>,
	float
>);

static_assert(are_types_same<
	type_at<2u>::template for_types_of<int, float, bool>,
	bool
>);

int main() {}