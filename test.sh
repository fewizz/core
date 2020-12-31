g++ -g -Wall -std=c++20 -o test test.cpp
if [[ $1 == "gdb" ]]; then
    gdb test
else
    if ! ./test
    then 
        echo "error"
        exit 1
    fi
fi

rm test