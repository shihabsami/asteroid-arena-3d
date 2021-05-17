#include <iostream>
#include <utility>

using std::cout;
using std::endl;
using std::forward;

struct vector3d {
    vector3d(int x, int y, int z) : x(x), y(y), z(z) {}

    int x;
    int y;
    int z;
};

struct transform {
    transform(vector3d p, int s) : position(p), size(s) {}

    vector3d position;
    int size;
};

template<typename T>
class component {
public:
    template<typename ... U>
    explicit component(U ... u);

    T data;
};

template<typename T>
template<typename ... U>
component<T>::component(U ... u) : data(forward<U>(u) ... ) {}

int main() {
    component<transform> c = component<transform>(vector3d(1.0, 1.0, 1.0), 1.0);
}
