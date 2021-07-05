#include "parameter_pack/index_of.hpp"

int main() {
	static_assert(u::index_of<int, float, int, double> == 1);
	static_assert(u::index_of<int, float, bool, double> == u::not_found);
}