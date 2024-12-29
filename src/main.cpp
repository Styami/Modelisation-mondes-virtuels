#include <iostream>
#include "scalarfield.hpp"

int main() {
    ScalarField scalarfield = ScalarField("data/heightmapper.png");
    Image gradient = scalarfield.laplacian();
    gradient.save("data/testGrad.png");
    // scalarfield.save("data/test.png");   
    return 0;
}