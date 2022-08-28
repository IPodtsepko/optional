#pragma once

#include <memory>

namespace {
namespace copy {

template <bool enable_v>
struct constructor {
  constexpr constructor() noexcept = default;
  constexpr constructor(const constructor&) = delete;
  constexpr constructor(constructor&&) = default;
  constexpr constructor& operator=(const constructor&) = default;
  constexpr constructor& operator=(constructor&&) = default;
};

template <>
struct constructor<true> {};

template <bool enable_v>
struct assignment {
  constexpr assignment() noexcept = default;
  constexpr assignment(const assignment&) = default;
  constexpr assignment(assignment&&) = default;
  constexpr assignment& operator=(const assignment&) = delete;
  constexpr assignment& operator=(assignment&&) = default;
};

template <>
struct assignment<true> {};

} // namespace copy

namespace move {

template <bool enable_v>
struct constructor {
  constexpr constructor() noexcept = default;
  constexpr constructor(const constructor&) = default;
  constexpr constructor(constructor&&) = delete;
  constexpr constructor& operator=(const constructor&) = default;
  constexpr constructor& operator=(constructor&&) = default;
};

template <>
struct constructor<true> {};

template <bool enable_v>
struct assignment {
  constexpr assignment() noexcept = default;
  constexpr assignment(const assignment&) = default;
  constexpr assignment(assignment&&) = default;
  constexpr assignment& operator=(const assignment&) = default;
  constexpr assignment& operator=(assignment&&) = delete;
};

template <>
struct assignment<true> {};

} // namespace move

} // anonymous namespace

namespace special_members {
template <typename T>
struct enable : private copy::constructor<std::is_copy_constructible_v<T>>,
                private copy::assignment<std::is_copy_assignable_v<T>>,
                private move::constructor<std::is_move_constructible_v<T>>,
                private move::assignment<std::is_move_assignable_v<T>> {};
} // namespace special_members
