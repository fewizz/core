#pragma once

namespace __type {

	template<typename From>
	struct copy_const_t {
		template<typename To>
		using type = To;
	};

	template<typename From>
	struct copy_const_t<const From> {
		template<typename To>
		using type = const To;
	};

}

template<typename From, typename To>
using copy_const = __type::copy_const_t<From>::template type<To>;