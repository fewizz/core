#pragma once

namespace __type {

	template<typename From>
	struct copy_ref_t {
		template<typename To>
		using type = To;
	};

	template<typename From>
	struct copy_ref_t<From&> {
		template<typename To>
		using type = To&;
	};

	template<typename From>
	struct copy_ref_t<From&&> {
		template<typename To>
		using type = To&&;
	};

}

template<typename From, typename To>
using copy_ref = __type::copy_ref_t<From>::template type<To>;