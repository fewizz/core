#pragma once

template<bool P>
struct if_satisfying {

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

template<auto Value>
struct if_value {

	template<auto OtherValue>
	struct equals_to;

	template<auto OtherValue>
	requires(Value == OtherValue)
	struct equals_to<OtherValue> {

		template<typename T>
		struct then {
			using otherwise_if = T;
		};
	};

	template<auto OtherValue>
	requires(Value != OtherValue)
	struct equals_to<OtherValue> {

		template<typename T>
		struct then {
			using otherwise_if = if_value;
		};
	};

};