#include <iostream>
#include <string>
#include <vector>
#include <wtu/ecs/entity-manager.hpp>

namespace wtu::ecs {
auto EntityManager::create_entity() -> std::expected<size_t, std::string> {
    auto mut_borrow_result = alive_entities.borrow_mut();
    if (!mut_borrow_result.has_value()) {
        return std::unexpected(
            "Failed to borrow mutable reference to alive_entities");
    }

    MutRef<std::vector<bool>> &entities = mut_borrow_result.value();

    auto free_slot = std::ranges::find(*entities, false);

    // Check if a free slot was found (not equal to end)
    if (free_slot != entities->end()) {
        *free_slot = true;
        return std::distance(entities->begin(), free_slot);
    }

    return std::unexpected(
        "Failed to find a free slot in the alive_entities array");
}

void EntityManager::destroy_entity(size_t entityId) {
    // Attempt to get a mutable reference to the `alive_entities` array
    auto mut_borrow_result = alive_entities.borrow_mut();

    if (!mut_borrow_result.has_value()) {
        return;
    }

    MutRef<std::vector<bool>> &entities = mut_borrow_result.value();

    entities->at(entityId) = false;
}

[[nodiscard]] auto EntityManager::is_alive(size_t entityId) const -> bool {
    auto entities = alive_entities.borrow();
    if (!entities.has_value()) {
        std::cerr << "Failed to borrow reference to alive_entities"
                  << std::endl;
        return false;
    }

    auto entity_ref = (*entities)->at(entityId);

    return entity_ref;
}
} // namespace wtu::ecs
