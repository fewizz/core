#pragma once

namespace __range {

template<typename Type>
concept has_iterator_member_function =
	requires(Type& o) { o.iterator(); };

} // __range