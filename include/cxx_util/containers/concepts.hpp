#pragma once

#include <compare>
#include <concepts>
#include <iterator>
#include <type_traits>

namespace util {

// part of 22.2.1, table 75
template <class X>
concept container =
requires(X a, X b, X r, X&& rv, typename X::iterator i, typename X::iterator j) {
    typename X::value_type;
    requires std::same_as<typename X::reference, typename X::value_type&>;
    requires std::same_as<typename X::const_reference, const typename X::value_type&>;
    requires std::forward_iterator<typename X::iterator>;
    requires std::forward_iterator<std::remove_const_t<typename X::const_iterator>>;
    requires std::convertible_to<typename X::iterator, typename X::const_iterator>;
    requires std::signed_integral<typename X::difference_type>;
    requires std::unsigned_integral<typename X::size_type>;

    requires std::default_initializable<X>;
    requires std::constructible_from<X>;

    { a.~X() } -> std::same_as<void>;

    { a.begin() } -> std::same_as<typename X::iterator>;
    { static_cast<const X>(a).begin() } -> std::same_as<typename X::const_iterator>;

    { a.end() } -> std::same_as<typename X::iterator>;
    { static_cast<const X>(a).end() } -> std::same_as<typename X::const_iterator>;

    { static_cast<const X>(a).cbegin() } -> std::same_as<typename X::const_iterator>;
    { static_cast<const X>(a).cend() } -> std::same_as<typename X::const_iterator>;

    { i <=> j } -> std::same_as<std::strong_ordering>;

    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;

    { a.swap(b) } -> std::same_as<void>;
    { swap(a, b) } -> std::same_as<void>;

    { r = a } -> std::same_as<X&>;

    { a.size() } -> std::same_as<typename X::size_type>;
    { a.empty() } -> std::convertible_to<bool>;
};

}