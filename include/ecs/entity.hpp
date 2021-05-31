#pragma once

#include "ecs/entt.hpp"
#include "world/world.hpp"

class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, World* world);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        T& component = world->registry.emplace<T>(entityHandle, std::forward(args)...);
        return component;
    }

    template<typename T>
    T& GetComponent() {
        return world->registry.get<T>(entityHandle);
    }

    template<typename T>
    bool HasComponent() {
        return world->registry.all_of<T>(entityHandle);
    }

private:
    entt::entity entityHandle{ entt::null };
    World* world = nullptr;
};