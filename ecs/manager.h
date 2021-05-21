#ifndef MANAGER_H
#define MANAGER_H

#include "entity.h"

#include <vector>
#include <utility>

using std::vector;
using std::forward;
using std::enable_if;
using std::is_base_of;

class manager {
public:
    static size_t base_entity_id;
    static size_t base_component_id;
    static vector<shared_ptr<entity>> entities;

    manager() = default;

    template<typename T>
    static size_t entity_type_id() {
        static size_t new_id = base_entity_id += 1;
        return new_id;
    }

    template<typename T>
    static size_t component_type_id() {
        static size_t new_id = base_component_id += 1;
        return new_id;
    }

    template<typename T, typename enable_if<is_base_of<entity, T>::value>::type* = nullptr>
    static shared_ptr<T> register_entity() {
        shared_ptr<T> e = make_shared<T>(entity_type_id<T>());
        entities.template emplace_back(e);
        return e;
    }

    template<typename T, typename enable_if<is_base_of<component, T>::value>::type* = nullptr, typename... U>
    static shared_ptr<T> register_component(const shared_ptr<entity>& e, U... u) {
        shared_ptr<T> c = make_shared<T>(forward<U>(u)... );
        e->assign_component(component_type_id<T>(), c);
        return c;
    }
};

size_t manager::base_component_id{};
size_t manager::base_entity_id{};
vector<shared_ptr<entity>> manager::entities{};

#endif // !MANAGER_H
