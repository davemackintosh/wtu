#include <wtu/ecs/component-array.hpp>

namespace wtu::ecs {
template <Component T>
template <typename... Args>
auto ComponentArray<T>::emplace(size_t entity_id, Args &&...args) -> T & {
    data[entity_id].emplace(std::forward<Args>(args)...);
    return *data[entity_id];
}

template <Component T>
auto ComponentArray<T>::get(size_t entity_id) -> std::optional<T> & {
    return data[entity_id];
}

template <Component T> void ComponentArray<T>::remove(size_t entity_id) {
    data[entity_id].reset();
}
} // namespace wtu::ecs
