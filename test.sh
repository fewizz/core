test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	if ! clang++ \
		--config ./compile_flags.txt \
		-fsanitize=undefined \
		-fsanitize=memory \
		-g \
		-o build/$1 \
		test/$1.cpp
	then
		echo "compilation error"
		exit 1
	fi

	if build/$1; then
		echo "success"
	else
		echo "didn't pass"
		exit 1
	fi
}

if [ ! -z $1 ]; then
	test $1
	exit
fi

test meta/type/array_extent
test meta/type/is_same_as

test meta/types/are_same
test meta/types/at_indices
test meta/types/common
test meta/types/erase_at_index
test meta/types/count_of_satisfying_predicate
test meta/types/indices_of_satisfying_predicate
test meta/types/are_exclusively_satisfying_predicates

test meta/values/of
test meta/values/permutations

test meta/elements/of
test meta/elements/one_of
test meta/elements/at_index

test expected
test ordered
test value_type
test begin_end
test to_range
test copy
test starts_with

test concat
test split
test transform
test zip