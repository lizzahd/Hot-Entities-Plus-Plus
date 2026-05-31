//
// Created by xerox on 3/13/2026.
//

#pragma once

#ifdef ENTITY_TYPES_HEADER
#include ENTITY_TYPES_HEADER
#else
/// Essentially the category the entity is in
enum class EntityBroadType {};

/// The specific type of entity
enum class EntityType {};
#endif

/// The abstract class everything handled in EntityManager needs to inherit from
class IEntity {
    public:
    virtual ~IEntity() = default;

    [[nodiscard]] virtual int getId() const = 0;

    /// Essentially the category the entity is in
    [[nodiscard]] virtual EntityBroadType getBroadType() const = 0;

    /// The specific type of entity
    [[nodiscard]] virtual EntityType getType() const = 0;

    /// EntityManager uses this to determine if it should be removed
    [[nodiscard]] virtual bool isDead() const = 0;

    [[nodiscard]] virtual int getLayer() const = 0;
    virtual void setLayer(int layer) = 0;

    /// Called once for each entity in EntityManager::update
    virtual void update() = 0;
    virtual void draw() const = 0;
    virtual void drawDebug() const = 0;
};
