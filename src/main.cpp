#include <iostream>
#include "heightField.hpp"

int main() {
    HeightField heightfield = HeightField("data/render.png", glm::vec3(0, 0, 0), glm::vec3(10, 10, 100));
    Image slope = heightfield.slope();
    // Image scalarGradBlur = scalarGrad.blur();
    slope.save("data/slop.png");
    // scalarfield.save("data/test.png");   
    return 0;
}