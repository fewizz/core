#pragma once

#include <algorithm>
#include <compare>
#include <corecrt.h>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include "../encoding/encoding.hpp"
#include "character_iterator.hpp"

namespace vw {

template<
    enc::encoding Encoding,
    class Allocator = std::allocator<typename Encoding::char_type>
>
struct basic_string {
    using value_type      = vw::character_view<Encoding>;
    using allocator_type  = Allocator;
    using size_type       = typename std::allocator_traits<allocator_type>::size_type;
    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator        = vw::character_iterator<Encoding>;
    using const_iterator  = const iterator;

private:
    Allocator m_allocator;
public:
    
    // construct/copy/destroy
    basic_string() : basic_string(Allocator()) {}

    explicit basic_string(const Allocator& a) : m_allocator{a} {}

    basic_string(const basic_string&) = default;
    basic_string(basic_string&&) = default;

    basic_string(const basic_string& that, size_type pos, const Allocator& a = Allocator());

    // iterators
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    // capacity
    size_type capacity() const;
    size_type length() const;

    void clear();
    bool empty() const;
    
    // element access
    const_reference operator[](size_type pos) const;
    reference       operator[](size_type pos);
    const_reference at(size_type n) const;
    reference       at(size_type n);

    const value_type front() const;
    value_type front();
    const value_type back() const;
    value_type back();

    // modifiers
    
};

}