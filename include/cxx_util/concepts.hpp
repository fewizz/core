#pragma once

#include <type_traits>

namespace util {

template < class T >
concept destructible = std::is_nothrow_destructible_v<T>;

template <class From, class To>
concept convertible_to =
    std::is_convertible_v<From, To> &&
    requires(std::add_rvalue_reference_t<From> (&f)()) {
        static_cast<To>(f());
    };


template<class T, class U>
  concept common_­reference_­with =
    same_­as<common_reference_t<T, U>, common_reference_t<U, T>> &&
    convertible_­to<T, common_reference_t<T, U>> &&
    convertible_­to<U, common_reference_t<T, U>>;

template < class T, class... Args >
concept constructible_from =
  destructible<T> && std::is_constructible_v<T, Args...>;

template< class T >
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

template < class T, class U >
concept common_reference_with =
  std::same_as<std::common_reference_t<T, U>, std::common_reference_t<U, T>> &&
  std::convertible_to<T, std::common_reference_t<T, U>> &&
  std::convertible_to<U, std::common_reference_t<T, U>>;

template< class LHS, class RHS >
concept assignable_from =
  std::is_lvalue_reference_v<LHS> &&
  common_reference_with<
    const std::remove_reference_t<LHS>&,
    const std::remove_reference_t<RHS>&> &&
  requires(LHS lhs, RHS&& rhs) {
    { lhs = std::forward<RHS>(rhs) } -> std::same_as<LHS>;
  };

template < class T >
concept movable =
  std::is_object_v<T> &&
  move_constructible<T> &&
  assignable_from<T&, T> &&
  swappable<T>;

template <class T>
concept copy_constructible =
  move_constructible<T> &&
  constructible_from<T, T&> && convertible_to<T&, T> &&
  constructible_from<T, const T&> && convertible_to<const T&, T> &&
  constructible_from<T, const T> && convertible_to<const T, T>;

template <class T>
concept copyable =
  copy_constructible<T> &&
  movable<T> &&
  assignable_from<T&, T&> &&
  assignable_from<T&, const T&> &&
  assignable_from<T&, const T>;

template <class T>
concept semiregular = copyable<T> && default_initializable<T>;

template <class T>
concept regular = semiregular<T> && equality_comparable<T>;

}