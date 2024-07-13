#pragma once

#include "./basic.hpp"
#include "./extensions.hpp"

namespace __range {

template<typename IndexType>
struct index_view : range_extensions<index_view<IndexType>>{

	struct iterator_t {
		IndexType index_;

		IndexType operator *() {
			return index_;
		}

		iterator_t& operator++ () {
			++index_;
			return *this;
		}

		friend bool operator == (iterator_t a, iterator_t b) {
			return a.index_ == b.index_;
		}
	};

	IndexType end_;

	iterator_t iterator() { return iterator_t{IndexType{}}; }
	iterator_t sentinel() { return iterator_t{end_}; }
};

template<typename IndexType>
index_view(IndexType) -> index_view<IndexType>;

} // __range