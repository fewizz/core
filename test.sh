test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	if ! clang++ -g -Wall \
		-std=c++20 -Iinclude/cxx_util \
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

test mem_address
test obj_representation
test byte_iterator
test bytes
test encoding/ascii
test encoding/utf8