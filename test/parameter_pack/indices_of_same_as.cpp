#include "parameter_pack/indices_of_same_as.hpp"
#include <utility>

int main() {
	static_assert(
		std::is_same_v<
			u::indices_of_same_as<int, float, int, bool, int>,
			std::index_sequence<1, 3>
		>
	);
}