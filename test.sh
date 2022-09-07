test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	declare -a additional_args

	if [[ $OS != Windows_NT ]]; then
		additional_args+=(-fsanitize=undefined)
		additional_args+=(-fsanitize=memory)
	fi

	if ! clang++ \
		--config ./compile_flags.txt \
		${additional_args[@]} \
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
		result=$?
		echo "failure"
		exit ${result}
	fi
}

if [ ! -z $1 ]; then
	test $1
	exit
fi

test type/array_extent
test type/is_same_as

test types/are_same
test types/at_indices
test types/common
test types/erase_at_index
test types/count_of_satisfying_predicate
test types/indices_of_satisfying_predicate
test types/exclusively_satisfy_predicates

test values/of
test values/permutations

test elements/of
test elements/one_of
test elements/at_index

test expected
test ordered

test iterators/to_range

test range/element_type
test range/value_type
test range/begin_end
test range/copy
test range/starts_with

test ranges/concat_view
test ranges/split_view
test ranges/transform_view
test ranges/zip_view