#pragma once

namespace __type {

	template<typename From>
	struct copy_volatile_t {
		template<typename To>
		using type = To;
	};

	template<typename From>
	struct copy_volatile_t<volatile From> {
		template<typename To>
		using type = volatile To;
	};

}

template<typename From, typename To>
using copy_volatile = __type::copy_volatile_t<From>::template type<To>;