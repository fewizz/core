#include <types.hpp>
#include <type.hpp>

static_assert(
	types<int>::exclusively_satisfy_predicates<
		count_of<is_same_as<int>.while_decayed> == 1
	>
);

static_assert(
	types<int, int, float>::exclusively_satisfy_predicates<
		count_of<is_same_as<int>.while_decayed> == 2,
		count_of<is_same_as<float>.while_decayed> == 1
	>
);

static_assert(
	! types<int, int, float, float>::exclusively_satisfy_predicates<
		count_of<is_same_as<int>.while_decayed> == 2,
		count_of<is_same_as<int>.while_decayed> == 1
	>
);


template<typename... Types>
concept contain_two_bools_and_int =
	types<Types...>::template exclusively_satisfy_predicates<
		count_of<is_same_as<bool>.while_decayed> == 2,
		count_of<is_same_as<int>.while_decayed> == 1
	>;

static_assert(contain_two_bools_and_int<bool, int, bool>);



#include <array.hpp>
#include <__range/of_element_type_satisfying_predicate.hpp>

static_assert(
	types<int, array<int, 10>>::exclusively_satisfy_predicates<
		count_of<is_same_as<int>.while_decayed> == 1,
		count_of<is_range_of_element_type_satisfying_predicate<is_same_as<int>.while_decayed>> == 1
	>
);


int main() {}