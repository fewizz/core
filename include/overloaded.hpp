#pragma once

// from cppreference
template<typename... Types>
struct overloaded : Types... {

	using Types::operator () ...;

	auto then(auto&& function) {
		return [&](auto&&... args) -> decltype(auto) {
			this->operator () (forward<decltype(args)>(args)...);
			return function();
		};
	}

};

template<typename... Types>
overloaded(Types&&...) -> overloaded<Types...>;