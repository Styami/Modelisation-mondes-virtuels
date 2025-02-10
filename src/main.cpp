#include "heightField.hpp"
#include "scalarfield.hpp"
#include <chrono>
#include <glm/fwd.hpp>
#include <iostream>

int main() {
    HeightField heightfield = HeightField("data/heightmap.png", glm::vec3(-5), glm::vec3(5));
    auto start = std::chrono::system_clock::now();
    // HeightField resBlur = HeightField(ScalarField(heightfield.blur()).blur(), glm::vec3(-5), glm::vec3(5));
    // HeightField resSmooth = HeightField(ScalarField(heightfield.smooth()).smooth(), glm::vec3(-5), glm::vec3(5));
    // resBlur.streamArea(4).saveStreamArea("data/streamAreaBlur.png");
    // resBlur.normGradient().saveNormGrad("data/normGradientBlur.png");
    // resBlur.laplacian().saveLaplace("data/laplacianBlur.png");
    // resSmooth.streamArea(4).saveStreamArea("data/streamAreaSmooth.png");
    // resSmooth.normGradient().saveNormGrad("data/normGradientSmooth.png");
    // resSmooth.laplacian().saveLaplace("data/laplacianSmooth.png");
    heightfield.thermalErode(0.0001).normalize().save("data/thermalErode.png");
    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
    std::cout << "temps d'éxécution : " << duration << " ms.\n";
    return 0;
}