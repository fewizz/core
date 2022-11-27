#pragma once

#include <type.hpp>
#include <if_satisfies.hpp>
#include <move.hpp>
#include <expression_of_type.hpp>

// from cppreference
template<typename... Types>
struct overloaded : Types... {

	using Types::operator () ...;

	template<typename Function>
	struct then_t {
		overloaded original_;
		Function function_;

		template<typename... Args>
		using original_return_type = decltype(
			original_(forward<Args>(expression_of_type<Args>)...)
		);

		template<typename... Args>
		static constexpr bool original_return_type_is_void =
			same_as<original_return_type<Args...>, void>;

		constexpr decltype(auto) operator () (auto&&... args)
		requires original_return_type_is_void<decltype(args)...> {
			original_(forward<decltype(args)>(args)...);
			return function_();
		}

		constexpr decltype(auto) operator () (auto&&... args) {
			return function_(
				original_(forward<decltype(args)>(args)...)
			);
		}
	};

	template<typename Function>
	constexpr auto then(Function&& function) && {
		return then_t<Function>{ move(*this), function };
	}

};

template<typename... Types>
overloaded(Types&&...) -> overloaded<Types...>;