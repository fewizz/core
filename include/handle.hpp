#pragma once

#include "./forward.hpp"
#include "./exchange.hpp"

template<typename Type>
struct handle_underlying_t;

template<typename Type>
struct handle;

template<typename Type>
struct handle_interface;

template<typename Type>
struct handle_interface_base {
protected:
	using underlying_type = typename handle_underlying_t<Type>::type;

	constexpr const underlying_type& underlying() const & {
		return ((const handle<Type>*)this)->underlying();
	}

	constexpr       underlying_type& underlying() &       {
		return ((      handle<Type>*)this)->underlying();
	}
};

template<typename Type>
struct handle : handle_interface<Type> {
	using underlying_type = typename handle_underlying_t<Type>::type;

protected:

	static constexpr underlying_type invalid_underlying
		= handle_underlying_t<Type>::invalid;

	underlying_type underlying_;

public:

	constexpr handle() : underlying_{ invalid_underlying } {}

	constexpr handle(underlying_type underlying) :
		underlying_{ underlying }
	{}

	constexpr handle(handle&& other) :
		underlying_{ exchange(other.underlying_, invalid_underlying) }
	{}

	constexpr handle(const handle& other) :
		underlying_{ other.underlying_ }
	{}

	handle& operator = (handle&& other) {
		underlying_ = exchange(other.underlying_, invalid_underlying);
		return *this;
	}

	handle& operator = (const handle& other) {
		underlying_ = other.underlying_;
		return *this;
	}

	constexpr const underlying_type& underlying() const { return underlying_; }
	constexpr       underlying_type& underlying()       { return underlying_; }

	constexpr void invalidate() {
		underlying_ = invalid_underlying;
	}

	constexpr bool is_invalid() const {
		return underlying_ == invalid_underlying;
	}

	constexpr bool is_valid() const {
		return underlying_ != invalid_underlying;
	}

	handle_interface<Type>* operator -> () {
		return static_cast<handle_interface<Type>*>(this);
	}

};