#define POST_INCREMENT_FROM_PRE_INCREMENT \
auto operator ++ (int) { \
	std::decay_t<decltype(*this)> copy { *this }; \
	++d; \
	return copy; \
}