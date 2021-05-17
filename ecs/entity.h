#ifndef ENTITY_H
#define ENTITY_H

#include "constants.h"

#include <iostream>
#include <bitset>
#include <array>
#include <ostream>

using std::size_t;
using std::bitset;
using std::array;
using std::ostream;

class entity {
public:
    explicit entity(size_t id);
    void assign_component(size_t c_id, size_t c_index);
    void remove_component(size_t c_id);
    friend ostream& operator<<(ostream& stream, const entity& e);

    const size_t id;
};

entity::entity(size_t id) : id(id) {
    c_indices.fill(-1);
}

void entity::assign_component(size_t c_id, size_t c_index) {
    c_mask.set(c_id);
}

void entity::remove_component(size_t c_id) {
    c_mask.reset(c_id);
    c_indices.at(c_id) = -1;
}

ostream& operator<<(ostream& stream, const entity& e) {
    stream << "id: " << e.id << ", component mask: " << e.c_mask;
    return stream;
}

#endif // !ENTITY_H
