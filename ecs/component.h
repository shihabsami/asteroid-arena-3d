#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <utility>

using std::size_t;
using std::forward;

class component {
public:
    component(size_t id);

    size_t id;
};

#endif // !COMPONENT_H
