#include <iostream>
#include "scalarfield.hpp"

int main() {
    ScalarField scalarfield = ScalarField("data/render.png");
    ScalarField scalarSmooth = ScalarField(scalarfield.smooth());
    // Image scalarGradBlur = scalarGrad.blur();
    scalarSmooth.save("data/Smooth.png");
    // scalarfield.save("data/test.png");   
    return 0;
}