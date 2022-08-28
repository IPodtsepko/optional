#pragma once

#include "details.h"
#include "special_members.h"

template <typename T>
class optional : private special_members::enable<T>,
                 private detail::copy_base<T, std::is_trivially_copyable_v<T>> {
  using base = detail::copy_base<T, std::is_trivially_copyable_v<T>>;
  using base::base;

public:
  constexpr optional() noexcept = default;
  constexpr optional(const optional&) = default;
  constexpr optional(nullopt_t) noexcept {}
  constexpr optional(optional&&) = default;

  ~optional() = default;

  optional& operator=(optional const&) = default;
  optional& operator=(optional&&) = default;
  optional& operator=(nullopt_t) noexcept {
    reset();
    return *this;
  }

  constexpr T* operator->() noexcept {
    return &this->m_value;
  }
  constexpr T const* operator->() const noexcept {
    return &this->m_value;
  }

  constexpr T& operator*() noexcept {
    return this->m_value;
  }
  constexpr T const& operator*() const noexcept {
    return this->m_value;
  }

  constexpr explicit operator bool() const noexcept {
    return this->m_active;
  }

  void reset() noexcept(std::is_nothrow_destructible_v<T>){
    this->base_reset();
  }

  template <typename... Args>
  void emplace(Args&&... args) {
    reset();
    this->m_value = T(std::forward<Args>(args)...);
    this->m_active = true;
  }
};

template <typename T>
constexpr bool operator<(optional<T> const& a, optional<T> const& b) {
  if (!b) {
    return false;
  }
  if (!a) {
    return true;
  }
  return *a < *b;
}

template <typename T>
constexpr bool operator==(optional<T> const& a, optional<T> const& b) {
  if (bool(a) != bool(b)) {
    return false;
  }
  if (!bool(a)) { // and so bool(b) == false
    return true;
  }
  return *a == *b;
}

template <typename T>
constexpr bool operator!=(optional<T> const& a, optional<T> const& b) {
  if (bool(a) != bool(b)) {
    return true;
  }
  if (!bool(a)) { // and so bool(b) == false
    return false;
  }
  return *a != *b;
}

template <typename T>
constexpr bool operator<=(optional<T> const& a, optional<T> const& b) {
  if (!bool(a)) {
    return true;
  }
  if (!bool(b)) {
    return false;
  }
  return *a <= *b;
}

template <typename T>
constexpr bool operator>(optional<T> const& a, optional<T> const& b) {
  if (!a) {
    return false;
  }
  if (!b) {
    return true;
  }
  return *a > *b;
}

template <typename T>
constexpr bool operator>=(optional<T> const& a, optional<T> const& b) {
  if (!bool(b)) {
    return true;
  }
  if (!bool(a)) {
    return false;
  }
  return *a >= *b;
}
