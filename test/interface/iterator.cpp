#include "../../include/cxx_util/interface/iterator.hpp"
#include <compare>
#include <cstddef>
#include <iterator>

int global;

struct f_iterator_test
: u::forward_iterator<f_iterator_test, u::value_type<int>> {
	using forward_iterator::operator ++;

	auto& operator * () const {
		return global;
	}

	auto& operator ++ () {
		return *this;
	}

	bool operator == (const f_iterator_test&) const {
		return false;
	}
};

static_assert(std::input_or_output_iterator<f_iterator_test>);
static_assert(std::forward_iterator<f_iterator_test>);

struct bi_iterator_test
: u::bidirectional_iterator<bi_iterator_test, u::value_type<int>> {
	using base_type = u::bidirectional_iterator<bi_iterator_test, u::value_type<int>>;

	using base_type::operator ++;
	using base_type::operator --;

	auto& operator * () const {
		return global;
	}

	auto& operator ++ () {
		return *this;
	}

	auto& operator -- () {
		return *this;
	}

	bool operator == (const bi_iterator_test&) const {
		return false;
	}
};

static_assert(std::bidirectional_iterator<bi_iterator_test>);

struct c_iterator_test
: u::contiguous_iterator<c_iterator_test, u::value_type<int>> {
	using base_type = u::contiguous_iterator<c_iterator_test, u::value_type<int>>;
	using base_type::operator-;

	int& operator * () const {
		return global;
	}

	auto& operator += (std::ptrdiff_t n) {
		return *this;
	}

	std::ptrdiff_t operator - (const c_iterator_test& that) const {
		return 0;
	}

	std::strong_ordering operator <=> (const c_iterator_test&) const {
		return std::strong_ordering::equal;
	}

	bool operator == (const c_iterator_test&) const {
		return true;
	}
};

c_iterator_test operator + (std::ptrdiff_t n, c_iterator_test it) {
	return { };
} 

static_assert(std::random_access_iterator<c_iterator_test>);
static_assert(std::contiguous_iterator<c_iterator_test>);

int main() {
	
}