test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	if ! clang++ \
		--config ./compile_flags.txt \
		-g \
		-o build/$1 \
		-fno-exceptions \
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

test 