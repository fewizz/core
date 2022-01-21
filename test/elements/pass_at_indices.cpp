#include "elements/pass_at_indices.hpp"
#include "type/is_same_as.hpp"
#include "../assert.hpp"

static_assert(
	type::is_same_as<
		int
	>::for_type<
		types::at_index<1>::template for_types<int, int, int, int, int>
	>
);

void f1(auto&&... values) {}

template<nuint... Indices>
struct a {

	template<typename... Types>
	static void f0(Types&&... values) {
		f1(
			((
				type::remove_reference::for_type<
					typename types::at_index<Indices>::template for_types<Types...>>&&
			) elements::at_index<Indices>::for_elements_of(values...) ) ...
		);
	}

};

int main() {

	a<0, 2, 4>::f0(0, 1, 2, 3, 4);

	int b = false;

	elements::pass_at_indices<3, 2, 0>(0, 1, 2, 3, 4)(
		[&](auto... val) {
			b = (val + ...);
		}
	);

		elements::pass_at_indices<3, 2, 0>::to_function{
		[&](auto... val) {
			b = (val + ...);
		}
	}.for_elements(elements::of{ 0, 1, 2, 3, 4 });

	assert(b == 8);

}