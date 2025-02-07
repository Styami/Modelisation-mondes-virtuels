#include "heightField.hpp"

int main() {
    ScalarField heightfield = ScalarField("data/heightmapper.png");
    heightfield.save("test.png");
    ScalarField resBlur = ScalarField(ScalarField(heightfield.blur()).blur());
    ScalarField resSmooth = ScalarField(ScalarField(heightfield.smooth()).smooth());
    resSmooth.save("pititTest.png");
    resBlur.streamArea(4).saveStreamArea("data/streamAreaBlur2.png");
    resBlur.normGradient().saveNormGrad("data/normGradientBlur2.png");
    resBlur.laplacian().saveLaplace("data/laplacianBlur2.png");
    resSmooth.streamArea(4).saveStreamArea("data/streamAreaSmooth2.png");
    resSmooth.normGradient().saveNormGrad("data/normGradientSmooth2.png");
    resSmooth.laplacian().saveLaplace("data/laplacianSmooth2.png");   
    return 0;
}