#include <body.hpp>
#include <integer.hpp>

struct something {
	int i;
};

template<>
struct handle_underlying_t<something> {
	using type = something*;
	static constexpr type invalid = nullptr;
};

template<>
struct handle_interface<something> {};

template<>
void body<something>::do_destroy() {
	soul_handle_.underlying()->i = -1;
}

int main() {
	something s{ 42 };

	{
		body<something>{ &s };
	}

	if(s.i != -1) {
		return 1;
	}

	return 0;
}
