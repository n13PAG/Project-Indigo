#ifndef COMPONENT_ARRAY_H_
#define COMPONENT_ARRAY_H_

#include "mcs.hpp"
#include "component.hpp"
#include <vector>

#include <assert.h>
#include <type_traits>
#include <concepts>
#include <unordered_map>

namespace MCS
{
    template <typename T>
    concept ConceptComponent = requires(T t) {
        { t.owner } -> std::convertible_to<EntityID>;
        { t.index } -> std::convertible_to<size_t>;
        { t.active } -> std::convertible_to<bool>;
    } && std::derived_from<T, Component>;

    template <ConceptComponent T>
    class ComponentArray
    {
    public:
        ComponentArray(size_t max_components)
            : _max_components(max_components),
              _available_components(max_components),
              _active_component_count(0),
              _components(max_components) {}

        ComponentArray()
            : ComponentArray(MAX_ENTITIES) {}

        void SetComponentIndex(T &component, size_t index)
        {
            component.index = index;
        }

        void SetComponentOwner(T &component, EntityID entity_id)
        {
            component.owner = entity_id;
        }

        void SetComponentActiveState(T &component, bool active)
        {
            component.active = active;
        }

        void SetComponent(T &component, EntityID entity_id, size_t index, bool active)
        {
            component.active = active;
            component.index = index;
            component.owner = entity_id;
        }

        size_t GetComponentIndex(const T &component)
        {
            return component.index;
        }

        size_t GetComponentCount()
        {
            return _active_component_count;
        }

        size_t GetAvailableComponentCount()
        {
            return _available_components;
        }

        size_t GetMaxComponentNum()
        {
            return _max_components;
        }

        void Clear()
        {
            _active_component_count = 0;
            _available_components = _max_components;
            _components = std::vector<T>(_max_components);
            _entity_to_index.clear();
        }

        T &AddComponent(EntityID entity_id, size_t &index)
        {
            // Triggers if : No more components can be registered
            assert(_available_components > 0 && "Max component count reached!");

            // Get index of first available component
            index = _max_components - _available_components;

            // Triggers if : Index is out of range
            assert(index < _components.size() && index >= 0 && "New component index is out of range");

            // Update array counters
            _active_component_count++;
            _available_components--;

            SetComponent(_components[index], entity_id, index, true);

            _entity_to_index[entity_id] = index;

            return _components[index];
        }

        void RemoveComponent(const size_t index)
        {
            // Triggers if : There are no component to remove
            assert(_active_component_count > 0 && "No components left to remove");

            size_t target_index = index;
            size_t last_component_index = _active_component_count - 1;

            // Triggers if : Getting a non-existent component
            assert(target_index >= 0 && target_index <= last_component_index && "No component to be removed at this index.");

            // Check if the component to be removed is the last component
            if (target_index != last_component_index)
            {

                SetComponentIndex(_components[last_component_index], target_index);
                SetComponentOwner(_components[last_component_index], _components[target_index].owner);
                SetComponentActiveState(_components[last_component_index], _components[target_index]);

                // Swap target component with last component
                std::swap(_components[target_index], _components[last_component_index]);

                _entity_to_index[_components[target_index].owner] = target_index;
            }

            _active_component_count--;
            _available_components++;
        }

        T& GetComponent(EntityID entity_id) {
            // Triggers if : The entity passed in does not have a component of this type
            assert(_entity_to_index.contains(entity_id) && "Entity does not have a of this type component.");
            size_t component_index = _entity_to_index[entity_id];
            return _components[component_index];
        }

        // Returns first element if component vector
        auto begin()
        {
            return _components.begin();
        }

        // Returns the next available slot in the array
        auto end()
        {
            return _components.begin() + _active_component_count;
        }

    private:
        size_t _available_components = MAX_ENTITIES;
        size_t _active_component_count = 0;
        size_t _max_components = MAX_ENTITIES;

        std::vector<T> _components;
        std::unordered_map<EntityID, size_t> _entity_to_index;
    };

}

#endif // COMPONENT_ARRAY_H_