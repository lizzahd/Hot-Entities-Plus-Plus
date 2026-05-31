//
// Created by xerox on 3/13/2026.
//

#pragma once

enum class EntityBroadType {
    Character,
    Interactable,
};

enum class EntityType {
    None,
    Player,
    Npc,
    Interactable,
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
