#ifndef MCS_H
#define MCS_H

#include <stdint.h>
#include <bitset>

namespace MCS {
    
    // Entity Constants
    using EntityID = std::uint32_t;
    static const EntityID MAX_ENTITIES = 800;

    // Component IDs
    // All new components must be included here
    
    enum ComponentType {
        TRANSFORM,
        COLLIDER,
        CIRCLE,
        BOX2D,
        RIGIDBODY,
        COMP_TYPE_NUM
    };

    static const uint8_t COMPONENT_TYPE_COUNT = COMP_TYPE_NUM;

    using Signature = std::bitset<COMPONENT_TYPE_COUNT>;
}

#endif /* MCS_H */