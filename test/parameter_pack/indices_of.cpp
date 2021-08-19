#include "parameter_pack/indices_of.hpp"
#include <utility>

int main() {
	static_assert(
		std::is_same_v<
			u::indices_of<int, float, int, bool, int>,
			std::index_sequence<1, 3>
		>
	);
}