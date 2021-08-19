#include "parameter_pack/indexes_of.hpp"
#include <utility>

int main() {
	static_assert(
		std::is_same_v<
			u::indexes_of<int, float, int, bool, int>,
			std::index_sequence<1, 3>
		>
	);
}