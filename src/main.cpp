#include "heightField.hpp"

int main() {
    ScalarField heightfield = ScalarField("data/heightmap.png");
    heightfield.save("test.png");
    ScalarField resBlur = ScalarField(ScalarField(heightfield.blur()).blur());
    ScalarField resSmooth = ScalarField(ScalarField(heightfield.smooth()).smooth());
    resSmooth.save("pititTest.png");
    resBlur.streamArea(5).save("data/streamAreaBlur2.png");
    resBlur.normGradient().save("data/normGradientBlur2.png");
    resBlur.laplacian().saveLaplace("data/laplacianBlur2.png");
    resSmooth.streamArea(5).save("data/streamAreaSmooth2.png");
    resSmooth.normGradient().save("data/normGradientSmooth2.png");
    resSmooth.laplacian().saveLaplace("data/laplacianSmooth2.png");   
    return 0;
}