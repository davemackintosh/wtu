#pragma once

#include <expected>
#include <string>
#include <vector>
#include <wtu/borrow-checker.hpp>
#include <wtu/constants.hpp>

namespace wtu::ecs {
class EntityManager {
  private:
    Borrowable<std::vector<bool>> alive_entities;

  public:
    EntityManager() : alive_entities(std::vector<bool>(MAX_ENTITIES, false)) {};

    auto create_entity() -> std::expected<size_t, std::string>;
    void destroy_entity(size_t entityId);
    [[nodiscard]] auto is_alive(size_t entityId) const -> bool;
};
} // namespace wtu::ecs
