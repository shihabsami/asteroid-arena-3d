#include <iostream>
#include <utility>

#include "manager.h"

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

int main() {
    shared_ptr<e_test> entity = manager::register_entity<e_test>();
    cout << manager::entities.size() << endl;

    manager::register_component<c_test>(entity, 0);
    cout << entity->component_data[manager::component_type_id<c_test>()] << endl;
}
