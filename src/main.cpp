#include <iostream>
#include "scalarfield.hpp"

int main() {
    ScalarField scalarfield = ScalarField("data/render.png");
    ScalarField scalarGrad = ScalarField(scalarfield.gradient());
    Image scalarGradBlur = scalarGrad.blur();
    scalarGradBlur.save("data/testGradBlurFrance.png");
    // scalarfield.save("data/test.png");   
    return 0;
}