clang++ -g -std=c++20 -o test test.cpp
if [[ $1 == "gdb" ]]; then
    gdb test
else
    ./test
fi

rm test