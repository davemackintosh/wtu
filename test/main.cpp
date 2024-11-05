#include <catch2/catch_test_macros.hpp>
#include <wtu/ecs/entity-manager.hpp>

TEST_CASE("Entity manager works as expected", "[entity-manager]") {
    auto entity_manager = wtu::ecs::EntityManager();

    REQUIRE(entity_manager.create_entity() ==
            std::expected<size_t, std::string>(0));
    REQUIRE(entity_manager.create_entity() ==
            std::expected<size_t, std::string>(1));
    entity_manager.destroy_entity(1);
    REQUIRE(entity_manager.create_entity() ==
            std::expected<size_t, std::string>(1));
}
