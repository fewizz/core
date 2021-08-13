test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	if ! $CXX \
		--config ./compile_flags.txt \
		-g \
		-o build/$1 \
		test/$1.cpp
	then
		echo "compilation error"
		exit 1
	fi

	if ! build/$1; then 
		echo "not passed"
		exit 1
	fi
}

if [ ! -z $1 ]; then
	test $1
	exit
fi

test iterator/referencing_iterator
test object/operations
test object/bits
test object/bytes
test parameter_pack/for_each
test parameter_pack/index_of
test parameter_pack/parameter_pack
test byte_iterator