#pragma once

namespace __range {
	
	template<typename Type>
	concept has_size_member_function =
		requires(Type& o) { o.size(); };

}