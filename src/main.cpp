#include "heightField.hpp"

int main() {
    ScalarField heightfield = ScalarField("data/render.png");
    ScalarField resBlur = ScalarField(ScalarField(heightfield.blur()).blur());
    ScalarField resSmooth = ScalarField(ScalarField(heightfield.smooth()).smooth());
    resBlur.streamArea(5).save("data/streamAreaBlur.png");
    resBlur.normGradient().save("data/normGradientBlur.png");
    resBlur.laplacian().save("data/laplacianBlur.png");
    resSmooth.streamArea(5).save("data/streamAreaSmooth.png");
    resSmooth.normGradient().save("data/normGradientSmooth.png");
    resSmooth.laplacian().save("data/laplacianSmooth.png");   
    return 0;
}