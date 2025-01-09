#include "heightField.hpp"
#include "scalarfield.hpp"

int main() {
    ScalarField heightfield = ScalarField("data/render.png");//, glm::vec3(0, 0, -10), glm::vec3(10, 10, 100));
    Image slope = heightfield.streamArea(4);
    // Image scalarGradBlur = scalarGrad.blur();
    Image res = ScalarField(slope).blur();
    Image res1 = ScalarField(res).blur();
    res.save("streamArea.png");
    // scalarfield.save("data/test.png");   
    return 0;
}