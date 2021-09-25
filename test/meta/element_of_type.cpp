#include "meta/element_of_type.hpp"

int main() {
	auto el = element_of_type<int>::template for_elements_of(0, 1.0, true);
	
}