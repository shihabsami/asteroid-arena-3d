#include <iostream>
#include <utility>

#include "data_manager.h"

using std::cout;
using std::endl;

class e_test : public entity {
public:
    explicit e_test(size_t id) : entity::entity(id) {}
};

class c_test : public component {
public:
    explicit c_test(int data) : data(data) {}

    int data{};
};

class c_test_alt : public component {
};

int main() {
    shared_ptr<e_test> entity = data_manager::register_entity<e_test>();
    cout << data_manager::entities.size() << endl;

    data_manager::register_component<c_test>(entity, 0);
    data_manager::register_component<c_test_alt>(entity);
    data_manager::register_component<c_test_alt>(entity);
    data_manager::register_component<c_test_alt>(entity);

    for (const auto& c : entity->component_data)
        cout << c.first << " - " << c.second << endl;

    cout << entity->component_register.size() << endl;
    cout << (data_manager::entity_has<c_test, c_test_alt>(entity) ? "y" : "n") << endl;

    entity->remove_component(1);
    for (const auto& c : entity->component_data)
        cout << c.first << " - " << c.second << endl;

    cout << (data_manager::entity_has<c_test>(entity) ? "y" : "n") << endl;
}
