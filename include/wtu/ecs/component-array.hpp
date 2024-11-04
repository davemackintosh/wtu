#pragma once

#include "wtu/borrow-checker.hpp"
#include "wtu/constants.hpp"
#include <array>
#include <optional>

namespace wtu::ecs {
/**
 * Concept to ensure components are Plain Old Data (POD) types and can be
 * borrowed safely without a copy or move.
 *
 * A Component can be borrowed using the `borrow()` and `borrow_mut()` methods
 * and it must be at most 128 bytes in size as this improves the ability for the
 * CPU to optimise the cache-line and improve cache hits.
 */
template <typename T>
concept Component =
    std::is_default_constructible_v<T> && std::is_trivially_destructible_v<T> &&
    requires(T target) {
        { target } -> std::same_as<const T &>; // Read-only borrow
        { target } -> std::same_as<T &>;       // Mutable borrow
    } && !std::is_copy_constructible_v<T> &&
    !std::is_move_constructible_v<T> && // Prevent copy/move
    (sizeof(T) <= 128); // Example: limit size to 128 bytes. NOLINT

template <Component T> class ComponentArray {
    Borrowable<std::array<T, MAX_COMPONENTS>> data;

  public:
    template <typename... Args>
    auto emplace(size_t entity_id, Args &&...args) -> T &;
    auto get(size_t entity_id) -> std::optional<T> &;
    void remove(size_t entity_id);
};
} // namespace wtu::ecs
