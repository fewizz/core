#pragma once

namespace __range {

	template<typename Type>
	concept has_sentinel_member_function =
		requires(Type& o) { o.sentinel(); };

}