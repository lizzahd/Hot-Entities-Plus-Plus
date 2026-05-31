//
// Created by xerox on 3/13/2026.
//

#pragma once

enum class EntityBroadType {
#ifdef ENTITY_BROAD_TYPES
    ENTITY_BROAD_TYPES
#else
    None,
#endif // ENTITY_BROAD_TYPES
};

enum class EntityType {
#ifdef ENTITY_TYPES
    ENTITY_TYPES
#else
    None,
#endif // ENTITY_TYPES
};

class IEntity {
    public:
    virtual ~IEntity() = default;
    [[nodiscard]] virtual int getId() const = 0;
    [[nodiscard]] virtual EntityBroadType getBroadType() const = 0;
    [[nodiscard]] virtual EntityType getType() const = 0;
    [[nodiscard]] virtual bool isDead() const = 0;

    [[nodiscard]] virtual int getLayer() const = 0;
    virtual void setLayer(int layer) = 0;

    virtual void update() = 0;
    virtual void draw() const = 0;
    virtual void drawDebug() const = 0;
};
