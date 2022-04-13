#pragma once

#include "declaration.hpp"

template<typename ObjectType>
struct guarded_handle_base {
	using object_type = ObjectType;

	handle<ObjectType> m_handle;

	const ::handle<ObjectType>& handle() const &  { return m_handle; }
	      ::handle<ObjectType>& handle()       &  { return m_handle; }
	      ::handle<ObjectType>  handle() const && { return m_handle; }
	      ::handle<ObjectType>  handle()       && { return m_handle; }

	decltype(auto) value() const { return handle().value(); }
	decltype(auto) value()       { return handle().value(); }

};