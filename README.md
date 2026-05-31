# Hot Entities Plus Plus
Flexible entity manager for game development

## Setup
This library has optional extended functionality
implemented via external headers

---

Firstly (and highly recommended), we have our header (`EntityTypes.h`) in which our entity types are declared  
```c++
// EntityTypes.h

/// The broad category the entity is in
enum class EntityBroadType {
    Character,
    Interactable
};

/// The specific type of entity
enum class EntityType {
    Player,
    Npc,
    Interactable
};
```

---

Then we have our header (`EntityManagerExternal.h`) in which we specify what types our Entity Manager will be using
```c++
// EntityManagerExternal.h

/// The types used by entity manager so it doesn't need external libraries
#define ENTITY_MANAGER_DECLARATIONS class AssetManager; \
                                    struct Camera2D;

/// The arguments EntityManager needs for its constructor
#define ENTITY_MANAGER_REQUIREMENTS const std::shared_ptr<AssetManager>& assetManager, \
                                    const std::shared_ptr<Camera2D>& camera

/// The values being passed to an entity upon its creation in EntityManager
#define ENTITY_MANAGER_MEMBERS      m_assetManager, \
                                    m_camera

/// The constructor fields for EntityManager
#define ENTITY_MANAGER_CONSTRUCTION : m_assetManager(assetManager) \
                                    , m_camera(camera)

/// The member declarations for EntityManager
#define ENTITY_MANAGER_MEMBERS_DECL std::shared_ptr<AssetManager> m_assetManager; \
                                    std::shared_ptr<Camera2D> m_camera;
```

---

Both of these are optional, but can be added to your projects CMakeLists.txt like so
```cmake
target_compile_definitions(PROJECT_NAME PRIVATE
    ENTITY_TYPES_HEADER=<EntityTypes.h>
    ENTITY_MANAGER_EXTERNAL_HEADER=<EntityManagerExternal.h>
)
```

> This README and all code in this project were made from scratch by a human