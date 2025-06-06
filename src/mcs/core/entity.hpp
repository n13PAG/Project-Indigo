#ifndef ENTITY_H_
#define ENTITY_H_

#include "mcs.hpp"
#include "component.hpp"

namespace MCS {
    using EntitySignature = Signature;
    
    struct Entity {
        EntityID id;
        EntitySignature signature;
        bool active;
    };
}

#endif