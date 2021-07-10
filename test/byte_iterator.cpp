#include "byte_iterator.hpp"
#include "iterator/iterator.hpp"
#include <bits/iterator_concepts.h>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <iostream>

using BIint = u::byte_iterator<int*>;

static_assert(std::contiguous_iterator<int*>);

static_assert(
	std::is_same_v<
		BIint::iterator_concept,
		std::contiguous_iterator_tag
	>
);

static_assert(std::input_iterator<BIint>);
static_assert(std::output_iterator<BIint, std::byte>);
static_assert(std::forward_iterator<BIint>);
static_assert(std::bidirectional_iterator<BIint>);
static_assert(std::random_access_iterator<BIint>);
static_assert(std::contiguous_iterator<BIint>);

#include <list>

using BIlist = u::byte_iterator<std::list<int>::iterator>;

static_assert(std::input_iterator<BIlist>);
static_assert(std::output_iterator<BIlist, std::byte>);
static_assert(std::forward_iterator<BIlist>);
static_assert(std::bidirectional_iterator<BIlist>);
static_assert(not std::random_access_iterator<BIlist>);
static_assert(not std::contiguous_iterator<BIlist>);

#include <forward_list>

using BIfl = u::byte_iterator<std::forward_list<int>::iterator>;

static_assert(std::input_iterator<BIfl>);
static_assert(std::output_iterator<BIfl, std::byte>);
static_assert(std::forward_iterator<BIfl>);
static_assert(not std::bidirectional_iterator<BIfl>);
static_assert(not std::random_access_iterator<BIfl>);
static_assert(not std::contiguous_iterator<BIfl>);

#include <set>

int main() {
	int arr[]{ 0x1, 0x2, 0x3, 0x4 };

	auto it = u::byte_iterator<int*, std::endian::little>{ arr };

	auto copy = it;
	assert((++copy).byte_index() == 1);

	assert((it + 4) - 4 == it);

	assert(
		std::distance(
			it,
			u::byte_iterator<int*, std::endian::little>{ arr + 4 }
		) == sizeof(int) * 4
	);

	assert(*it                   == std::byte{ 0x1 });
	assert(*(it + 1*sizeof(int)) == std::byte{ 0x2 });
	assert(*(it + 2*sizeof(int)) == std::byte{ 0x3 });
	assert(*(it + 3*sizeof(int)) == std::byte{ 0x4 });

	std::set<int> s{ 0x1, 0x2, 0x3, 0x4 };
	using set_it = typename std::set<int>::iterator;
	auto it2 = u::byte_iterator<set_it, std::endian::big>{ s.begin() };

	std::advance(it2, 3); // because of endian::big
	assert(*it2 == std::byte{ 0x1 });
	std::advance(it2, sizeof(int));
	assert(*it2 == std::byte{ 0x2 });
	std::advance(it2, sizeof(int));
	assert(*it2 == std::byte{ 0x3 });
	std::advance(it2, sizeof(int));
	assert(*it2 == std::byte{ 0x4 });

	*it2 = std::byte{ 42 };
	assert(*it2 == std::byte{ 42 });
}