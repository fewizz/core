#include "number.hpp"
#include <stdio.h>

int main() {
	for_each_digit_in_number(number{ 1122 }, base{ 10 }, [](uint d) {
		putc(d + '0', stdout);
	});

	putc('\n', stdout);

	for_each_digit_in_number(number{ 65536 }, base{ 2 }, [](uint d) {
		putc(d + '0', stdout);
	});

	putc('\n', stdout);

	for_each_digit_in_number(number{ 65535 }, base{ 16 }, [](uint d) {
		putc(d + (d < 10 ? '0' : 'a' - 10), stdout);
	});

	putc('\n', stdout);
}