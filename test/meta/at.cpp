#include "meta/at.hpp"

static_assert(are_same<
	at<0u>::template of_types<int, float, bool>,
	int
>);

static_assert(are_same<
	at<1u>::template of_types<int, float, bool>,
	float
>);

static_assert(are_same<
	at<2u>::template of_types<int, float, bool>,
	bool
>);

int main() {}