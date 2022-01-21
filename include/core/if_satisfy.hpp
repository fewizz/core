#pragma once

template<bool P>
struct if_satisfy {

	template<typename T>
	struct then;

	template<typename T>
	requires(P)
	struct then<T> {
		template<typename E>
		using otherwise = T;
	};

	template<typename T>
	requires(!P)
	struct then<T> {
		template<typename E>
		using otherwise = E;
	};
};