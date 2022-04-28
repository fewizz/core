#pragma once

template<
	typename ValueType, typename SizeType, typename Grower
>
class vector {
	using value_type = ValueType;
	using size_type = SizeType;

	ValueType* ptr_ = 0;
	SizeType size_ = 0;
	SizeType allocated_ = 0;
	Grower grower_;

	constexpr void grow() {
		grower_.grow(ptr_, size_, allocated_);
	}
public:

	constexpr vector(Grower&& grower) : grower_{ grower } {}

	constexpr size_type size() { return size_; }

	constexpr value_type* data() { return ptr_; }
	constexpr const value_type* data() const { return ptr_; }

	constexpr value_type* begin() { return data(); }
	constexpr const value_type* begin() const { return data(); }

	constexpr value_type* end() { return begin() + size(); }
	const value_type* end() const {return begin() + size(); }

	void push_back(value_type&& o) {
		if(size_ == allocated_) grow();
		new (*ptr_) value_type(move(o));
		++size_;
	}

	constexpr ~vector() {
		for(size_type i = size(); i > 0;) {
			data()[--i].~value_type();
		}

		grower_.deallocate(ptr_, allocated_);
	}

};