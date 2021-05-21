#ifndef ENTITY_H
#define ENTITY_H

#include "component.h"

#include <unordered_map>
#include <memory>

using std::unordered_map;
using std::shared_ptr;
using std::make_shared;

class entity {
public:
    inline explicit entity(size_t id) : id(id) {}
    inline ~entity() { component_data.clear(); }
    void assign_component(size_t c_id, const shared_ptr<component>& c);
    void remove_component(size_t c_id);

    const size_t id;
    unordered_map<size_t, bool> component_register;
    unordered_map<size_t, shared_ptr<component>> component_data;
};

inline void entity::assign_component(size_t c_id, const shared_ptr<component>& c) {
    if (!component_register[c_id]) {
        component_register[c_id] = true;
        component_data[c_id] = c;
    }
}

inline void entity::remove_component(size_t c_id) {
    if (component_register[c_id]) {
        component_register[c_id] = false;
        component_data[c_id].reset();
    }
}

#endif // !ENTITY_H
