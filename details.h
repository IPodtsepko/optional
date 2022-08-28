#pragma once

#include <memory>

#include "optional_consts.h"

namespace detail {

struct dummy_t {};

template <typename T, bool trivial>
struct destroy_base {
  constexpr destroy_base() noexcept : m_dummy{} {}

  constexpr destroy_base(const T& value) : m_value(value), m_active(true) {}

  constexpr destroy_base(T&& value) : m_value(std::move(value)), m_active(true) {}

  template <typename... Args>
  constexpr destroy_base(in_place_t, Args&&... args)
      : m_active(true), m_value(std::forward<Args>(args)...) {}

  void base_reset() noexcept(std::is_nothrow_destructible_v<T>) {
    if (!m_active) {
      return;
    }
    m_value.~T();
    m_active = false;
  }

  ~destroy_base() {
    this->base_reset();
  }

  union {
    T m_value;
    dummy_t m_dummy;
  };
  bool m_active{false};
};

template <typename T>
struct destroy_base<T, true> {
  constexpr destroy_base() noexcept : m_dummy{} {}

  constexpr destroy_base(const T& value) : m_value(value), m_active(true) {}

  constexpr destroy_base(T&& value) : m_value(std::move(value)), m_active(true) {}

  template <typename... Args>
  constexpr destroy_base(in_place_t, Args&&... args)
      : m_active(true), m_value(std::forward<Args>(args)...) {}

  void base_reset() noexcept(std::is_nothrow_destructible_v<T>) {
    if (!m_active) {
      return;
    }
    m_value.~T();
    m_active = false;
  }

  ~destroy_base() = default;

  union {
    T m_value;
    dummy_t m_dummy;
  };
  bool m_active{false};
};

template <typename T, bool trivial>
struct copy_base : destroy_base<T, std::is_trivially_destructible_v<T>> {
  using base = destroy_base<T, std::is_trivially_destructible_v<T>>;
  using base::base;

  constexpr copy_base() noexcept = default;

  constexpr copy_base(const copy_base& other) {
    if (other.m_active) {
      new (&(this->m_value)) T(other.m_value);
    }
    this->m_active = other.m_active;
  }

  constexpr copy_base(copy_base&& other) noexcept {
    if (other.m_active) {
      new (&(this->m_value)) T(std::move(other.m_value));
    }
    this->m_active = other.m_active;
  }

  constexpr copy_base& operator=(const copy_base& other) {
    if (this->m_active && !other.m_active) {
      this->base_reset();
    } else if (!this->m_active && other.m_active) {
      new (&(this->m_value)) T(other.m_value);
    } else if (this->m_active && other.m_active) {
      this->m_value = other.m_value;
    }
    this->m_active = other.m_active;
    return *this;
  }

  constexpr copy_base& operator=(copy_base&& other) {
    if (this->m_active && !other.m_active) {
      this->base_reset();
    } else if (!this->m_active && other.m_active) {
      new (&(this->m_value)) T(std::move(other.m_value));
    } else if (this->m_active && other.m_active) {
      this->m_value = std::move(other.m_value);
    }
    this->m_active = other.m_active;
    return *this;
  }
};

template <typename T>
struct copy_base<T, true> : destroy_base<T, std::is_trivially_destructible_v<T>> {
  using base = destroy_base<T, std::is_trivially_destructible_v<T>>;
  using base::base;

  constexpr copy_base() noexcept = default;
  constexpr copy_base(const copy_base&) = default;
  constexpr copy_base(copy_base&&) = default;
  constexpr copy_base& operator=(const copy_base&) = default;
  constexpr copy_base& operator=(copy_base&&) = default;
};

} // namespace detail
