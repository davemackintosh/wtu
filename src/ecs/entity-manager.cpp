#include <wtu/ecs/entity-manager.hpp>

namespace wtu::ecs {
auto EntityManager::create_entity() -> std::optional<size_t> {
    auto mut_borrow_result = alive_entities.borrow_mut();
    if (!mut_borrow_result.has_value()) {
        return std::nullopt;
    }

    MutRef<std::array<bool, MAX_ENTITIES>> &entities = *mut_borrow_result;

    auto *free_slot = std::ranges::find(*entities, false);

    // Check if a free slot was found (not equal to end)
    if (free_slot != entities->end()) {
        *free_slot = true;
        return std::distance(entities->begin(), free_slot);
    }

    return std::nullopt;
}

void EntityManager::destroy_entity(size_t entityId) {
    // Attempt to get a mutable reference to the `alive_entities` array
    auto mut_borrow_result = alive_entities.borrow_mut();

    if (!mut_borrow_result) {
        return;
    }

    // Dereference the expected result to get the `MutRef` to the array
    MutRef<std::array<bool, MAX_ENTITIES>> &entities = *mut_borrow_result;

    entities->at(entityId) = false;
}

[[nodiscard]] auto EntityManager::is_alive(size_t entityId) const -> bool {
    auto entities = alive_entities.borrow().value();
    auto entity_ref = entities->at(entityId);

    return entity_ref;
}
} // namespace wtu::ecs
