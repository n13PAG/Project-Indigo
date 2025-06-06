#ifndef COMPONENT_H
#define COMPONENT_H

#include "mcs.hpp"

namespace MCS {
    using ComponentSignature = Signature;

    struct Component {
        ComponentType type;
        ComponentSignature signature;
        size_t index;
        EntityID owner;
        bool is_active;
    };
}

#endif /* COMPONENT_H */