#pragma once

#include <array>
#include <wtu/borrow-checker.hpp>
#include <wtu/constants.hpp>

namespace wtu::ecs {
class EntityManager {
  private:
    Borrowable<std::array<bool, MAX_ENTITIES>> alive_entities;

  public:
    auto create_entity() -> std::optional<size_t>;
    void destroy_entity(size_t entityId);
    [[nodiscard]] auto is_alive(size_t entityId) const -> bool;
};
} // namespace wtu::ecs
