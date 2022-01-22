#pragma once

namespace wrapper {

	template<typename T, typename = void>
	struct of {
		T m_t;

		explicit operator const T& () const {
			return m_t;
		}

		explicit operator T& () {
			return m_t;
		}
	};
}