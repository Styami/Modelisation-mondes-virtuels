#include <iostream>
#include "scalarfield.hpp"

int main() {
    ScalarField scalarfield = ScalarField("data/heightmapper.png");
    scalarfield.save("data/test.png");   
    return 0;
}