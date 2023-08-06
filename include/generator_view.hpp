#pragma once

#include "./integer.hpp"
#include "./__type/is_invokable_with.hpp"
#include "./__range/extensions_declaration.hpp"

template<typename Generator>
class generator_view : public range_extensions<generator_view<Generator>> {
	Generator generator_;
	nuint size_;

public:
	generator_view(
		Generator generator,
		nuint size
	) : generator_{generator}, size_{size} {}

	struct iterator_t {
		Generator generator_;
		nuint index = 0;

		decltype(auto) operator * () const {
			if constexpr(invokable_with<Generator, nuint>) {
				return generator_(index);
			}
			else {
				return generator_();
			}
		}

		iterator_t& operator ++ () {
			++index;
			return *this;
		}

		friend bool operator == (iterator_t a, iterator_t b) {
			return a.index == b.index;
		}

	};

	iterator_t iterator() { return { generator_, 0 }; }
	iterator_t sentinel() { return { generator_, size_ }; }

	nuint size() const { return size_; }

};

template<typename Generator>
generator_view(Generator gen) -> generator_view<Generator>;