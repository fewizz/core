#pragma once

#include <iterator>

namespace u {

template<typename It>
class referencing_iterator {
	It* it;

public:
	referencing_iterator() = default;
	referencing_iterator(const referencing_iterator& other) = default;
	referencing_iterator(referencing_iterator&& other) = default;
	referencing_iterator(It& it) : it{&it} {}

	referencing_iterator& operator = (const referencing_iterator& other) = default;

	using iterator_concept = std::input_iterator_tag;
	using difference_type = std::iter_difference_t<It>;
	using value_type = std::iter_value_t<It>;

	decltype(auto) operator * () const {
		return **it;
	}

	auto& operator ++ () {
		++*it;
		return *this;
	}

	auto operator ++ (int) {
		referencing_iterator copy{ *this };
		++*it;
		return copy;
	}

	bool operator == (const referencing_iterator& other) const {
		return *it == *it;
	}
};

}