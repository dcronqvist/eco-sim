#include "ecs/entity.hpp"

Entity::Entity(entt::entity handle, World* _world)
    : entityHandle(handle), world(_world) {

}