#include "../../include/cxx_util/interface/iterator.hpp"
#include <compare>
#include <cstddef>
#include <iterator>

struct f_iterator
: u::forward_iterator<f_iterator, u::value_type<int>> {
	using forward_iterator::operator ++;

	int& operator * () const;
	f_iterator& operator ++ ();
	bool operator == (const f_iterator&) const;
};

static_assert(std::input_or_output_iterator<f_iterator>);
static_assert(std::forward_iterator<f_iterator>);

struct bi_iterator
: u::bidirectional_iterator<bi_iterator, u::value_type<int>> {
	using base_type = u::bidirectional_iterator<bi_iterator, u::value_type<int>>;

	using base_type::operator ++;
	using base_type::operator --;

	int& operator * () const;
	bi_iterator& operator ++ ();
	bi_iterator& operator -- ();
	bool operator == (const bi_iterator&) const;
};

static_assert(std::bidirectional_iterator<bi_iterator>);

struct c_iterator
: u::contiguous_iterator<c_iterator, u::value_type<int>> {
	using base_type = u::contiguous_iterator<c_iterator, u::value_type<int>>;
	using base_type::operator-;

	int& operator * () const;
	c_iterator& operator += (std::ptrdiff_t n);
	std::ptrdiff_t operator - (const c_iterator& that) const;
	std::strong_ordering operator <=> (const c_iterator&) const;
	bool operator == (const c_iterator&) const;
};

c_iterator operator + (std::ptrdiff_t n, c_iterator it);

static_assert(std::random_access_iterator<c_iterator>);
static_assert(std::contiguous_iterator<c_iterator>);

int main() {
	
}