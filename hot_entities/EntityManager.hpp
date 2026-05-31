//
// Created by xerox on 3/13/2026.
//

#pragma once

#include <memory>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <map>

#include "IEntity.hpp"

class EntityManager : public std::enable_shared_from_this<EntityManager> {
public:
#ifdef ENTITY_MANAGER_REQUIREMENTS
    explicit EntityManager(ENTITY_MANAGER_REQUIREMENTS)
#ifdef ENTITY_MANAGER_CONSTRUCTION
        ENTITY_MANAGER_CONSTRUCTION
#endif // ENTITY_MANAGER_CONSTRUCTION
    {}
#else // ENTITY_MANAGER_REQUIREMENTS
    explicit EntityManager()
    {}
#endif // ENTITY_MANAGER_REQUIREMENTS
    ~EntityManager() = default;

    /// Takes a broad type and registers it to cover a list of typename(Class)'s
    template<typename I>
    void registerBroadType(const EntityBroadType broadType, const I typeIndices) {
        m_broadTypeRegistry.emplace(broadType, typeIndices);
    }

    /// Takes a broad type and registers it to cover a variadic amount of typename(Class)'s
    template<typename... Args>
    void registerBroadType(const EntityBroadType broadType, Args&&... args) {
        registerBroadType(broadType, std::vector<std::type_index>{std::type_index(args)...});
    }

    template<typename T, typename... Args>
    T* create(Args&&... args) {
        const int id = m_nextId++;
#ifdef ENTITY_MANAGER_MEMBERS
        auto entity = std::make_unique<T>(id, shared_from_this(), ENTITY_MANAGER_MEMBERS, std::forward<Args>(args)...);
#else
        auto entity = std::make_unique<T>(id, shared_from_this(), std::forward<Args>(args)...);
#endif
        T* ptr = entity.get();
        m_entities[id] = std::unique_ptr<IEntity>(std::move(entity));

        addEntityByBroadType(ptr);

        return ptr;
    }

    template<typename T>
    T* create(std::unique_ptr<T> entity) {
        T* ptr = entity.get();
        m_entities[ptr->getId()] = std::unique_ptr<IEntity>(std::move(entity));

        addEntityByBroadType(ptr);

        return ptr;
    }

    template<typename T, typename... Args>
    std::unique_ptr<T> createObject(Args&&... args) {
#ifdef ENTITY_MANAGER_MEMBERS
        return std::make_unique<T>(m_nextId++, shared_from_this(), ENTITY_MANAGER_MEMBERS, std::forward<Args>(args)...);
#else
        return std::make_unique<T>(m_nextId++, shared_from_this(), std::forward<Args>(args)...);
#endif
    }

    IEntity* get(const int id) {
        const auto it = m_entities.find(id);
        return it != m_entities.end() ? it->second.get() : nullptr;
    }

    template<typename T>
    T* getAs(const int id) {
        return dynamic_cast<T*>(get(id));
    }

    void remove(const int id) {
        const auto it = m_entities.find(id);
        if (it == m_entities.end()) {
            return;
        }

        // Layer Scheisse
        const int layer = it->second->getLayer();
        auto &vec = m_layerIndex[layer];
        std::erase(vec, id);

        m_entities.erase(id);
        for (auto& entityIds : m_entitiesByType | std::views::values) {
            std::erase(entityIds, id);
        }
    }

    void updateAll() {
        std::vector<int> toRemove;
        for (const auto& [id, entity]: m_entities) {
            if (entity->isDead()) {
                toRemove.push_back(id);
                continue;
            }

            entity->update();
        }

        // Gotta draw separately due to layering. A bit expensive
        for (const auto &ids: m_layerIndex | std::views::values) {
            for (const int id : ids) {
                m_entities[id]->draw();
            }
        }

        for (const int id : toRemove) {
            remove(id);
        }
    }

    template<typename T>
    std::vector<int>& getEntitiesByType() {
        return m_entitiesByType[std::type_index(typeid(T))];
    }

    template<typename F>
    void exec(F&& cb) {
        for (const auto &[id, entity]: m_entities) {
            cb(id, entity.get());
        }
    }

    template<typename T, typename F>
    void execByType(F&&cb) {
        for (int id : getEntitiesByType<T>()) {
            if (auto* entity = getAs<T>(id)) {
                cb(id, entity);
            }
        }
    }

    template<typename T, typename F>
    bool breakableExecByType(F&&cb) {
        for (int id : getEntitiesByType<T>()) {
            if (auto* entity = getAs<T>(id)) {
                if (cb(id, entity)) {
                    return true;
                }
            }
        }

        return false;
    }

    // Layer Scheisse
    void setLayer(const int id, const int layer) {
        const auto it = m_entities.find(id);
        if (it == m_entities.end()) {
            return;
        }

        const int oldLayer = it->second->getLayer();
        if (oldLayer == layer) {
            return;
        }

        auto &oldVec = m_layerIndex[oldLayer];
        std::erase(oldVec, id);

        it->second->setLayer(layer);
        m_layerIndex[layer].push_back(id);
    }

    int m_nextId = 0;
private:
    template<typename T>
    void addEntityByBroadType(T* ptr) {
        // Layer Scheisse
        const int id = ptr->getId();
        const int layer = ptr->getLayer();
        m_layerIndex[layer].push_back(id);

        const auto it = m_broadTypeRegistry.find(ptr->getBroadType());
        if (it != m_broadTypeRegistry.end()) {
            for (const auto type : it->second) {
                m_entitiesByType[type].push_back(id);
            }
        }
    }

    std::unordered_map<int, std::unique_ptr<IEntity>> m_entities;

    // This is so I can just find all structures or units really fast
    std::unordered_map<std::type_index, std::vector<int>> m_entitiesByType;
    // TODO: cast enum classes to int and store them as such, so they can use whatever enumerations they want
    std::unordered_map<EntityBroadType, std::vector<std::type_index>> m_broadTypeRegistry;

    std::map<int, std::vector<int>> m_layerIndex;

#ifdef ENTITY_MANAGER_MEMBERS_DECL
    ENTITY_MANAGER_MEMBERS_DECL;
#endif
};