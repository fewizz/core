#include "elements/at_index.hpp"
#include <cassert>

int main() {
	assert(elements::at_index<1>(0, 1.0F) == 1.0F);
}