#ifndef MANAGER_H
#define MANAGER_H

#include "entity.h"
#include "component.h"

#include <vector>
#include <deque>

using std::vector;
using std::deque;

namespace manager {
    static size_t base_entity_id = -1;
    static size_t base_component_id = -1;

    static vector<entity> entities;
    static vector<component> components;

    static deque<size_t> available_entity_ids;
    static deque<size_t> available_component_ids;

    template<typename T>
    size_t get_entity_id() {
        static size_t new_id = base_entity_id += 1;
        return new_id;
    }

    template<typename T>
    void get_component_id() {
        static size_t new_id = base_component_id += 1;
        return new_id;
    }

    template<typename T>
    size_t register_entity() {

    }

    template<typename T>
    void register_component(size_t e_index) {

    }

    template<typename T>
    vector<entity> get_entities() {
    }

    template<typename T>
    void assign(size_t e_index) {
    }
}

#endif // !MANAGER_H
