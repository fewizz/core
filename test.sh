test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	if ! clang++ \
		--config ./compile_flags.txt \
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

test mem_address
test obj_representation
test byte_iterator
test byte_range
test encoding/ascii
test encoding/utf8
test encoding/utf16
test byte_range
test character_iterator
test string_view