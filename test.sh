test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)

	declare -a additional_args

	if [[ $OS != Windows_NT ]]; then
		``
		#additional_args+=(-fsanitize=undefined)
		#additional_args+=(-fsanitize=memory)
	fi

	if ! clang++ \
		-std=c++23 \
		-nostdinc++ \
		-nostdinc \
		-Iinclude \
		-Wall \
		-Wextra \
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

for testfile in `cd 'test' && find -type f -name "*.cpp" && cd ..`; do
	test "`dirname $testfile`/`basename $testfile .cpp`"
done