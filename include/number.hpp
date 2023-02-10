#pragma once

#include "./type.hpp"
#include "./types.hpp"

class number_base {
	const nuint value_;
public:
	number_base(nuint value) : value_{ value } {}
	operator nuint () const { return value_; }
};

template<typename Type>
struct number {
	Type value_;

	template<typename Handler>
	void for_each_digit(number_base base, Handler&& handler) const {
		nuint divisor = 1;

		{
			Type number_copy = value_;

			while((number_copy /= base) > 0) {
				divisor *= base;
			}
		}

		{
			Type number_copy = value_;

			while(divisor > 0) {
				auto d = (number_copy / divisor) % base;
				handler(d);
				divisor /= base;
			}
		}
	}

	Type absolute() const {
		return value_ < Type{} ? -value_ : value_;
	}

	Type clamp(Type min, Type max) const {
		return value_ < min ? min : value_ > max ? max : value_;
	}

	Type align(Type alignment) {
		Type rem = value_ % alignment;
		if(rem == 0) {
			return value_;
		}
		return value_ + (alignment - rem);
	}

};

template<typename Type>
number(Type) -> number<Type>;