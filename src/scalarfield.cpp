#include "scalarfield.hpp"
#include <algorithm>
#include <cmath>
#include <array>
#include <utility>

ScalarField::ScalarField(const std::string& filename) :
    image(filename)
{}

ScalarField::ScalarField(const Image& image) :
    image(image)
{}

ScalarField::ScalarField(ScalarField&& scalarfield) :
    image(std::move(scalarfield.image))
{}


void ScalarField::save(const std::string& filename) {
    image.save(filename);
}

Image&& ScalarField::clamp(Image& image, int valueMin) const {
    for (int j = 0; j < image.getHeight(); j++)
        for (int i = 0; i < image.getWidth(); i++) {
            if(image[i, j][0] < valueMin)
                image.setData(i, j, glm::vec3(0));
        }
    return std::move(image);
}

Image ScalarField::normGradient() const {
    Image res(image.getWidth(), image.getHeight());

    for (int y = 0; y < image.getHeight(); y++) 
        for(int x = 0; x < image.getWidth(); x++) {
            float gradientRes = gradient(x, y) ;
            res.setData(x, y,  glm::vec3(gradientRes));
        }
    return res;
}

float ScalarField::gradient(const int x, const int y) const {
    glm::mat3 gradY = glm::mat3(-1, 0, 1,
                    -1, 0, 1,
                    -1, 0, 1
    );
    glm::mat3 gradX = glm::mat3(-1, -1, -1,
                    0, 0, 0,
                    1, 1, 1
    );

    float valueX = 0;
    float valueY = 0;
    for (int j = y - 1; j <= y + 1; j++) {
        int usingJ = std::clamp(j, 0, image.getHeight() - 1);
        for (int i = x - 1; i <= x + 1; i++) {
            int usingI = std::clamp(i, 0, image.getWidth() - 1);
            valueX += image[usingI, usingJ][0] * gradX[i - (x - 1)][j - (y - 1)];
            valueY += image[usingI, usingJ][0] * gradY[i - (x - 1)][j - (y - 1)];
        }
    }
    return sqrt(valueX * valueX + valueY * valueY);
}

Image ScalarField::laplacian() const {
    glm::mat3 laplacianKer = glm::mat3(0, 1, 0,
                                    1, -4, 1,
                                    0, 1, 0
    );

    Image res(image.getWidth(), image.getHeight());

    for (int y = 1; y < image.getHeight() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    int usingI = std::clamp(i, 0, image.getWidth() - 1);
                    int usingJ = std::clamp(j, 0, image.getHeight() - 1);
                    newColor += image[usingI, usingJ] * laplacianKer[i - (x - 1)][j - (y - 1)];
                }
                
            res.setData(x, y, newColor);
        }
    res.normalize();
    return res;
}

Image ScalarField::blur() const {
        glm::mat3 blurKer = glm::mat3(1, 1, 1,
                                    1, 1, 1,
                                    1, 1, 1
    );

    Image res(image.getWidth(), image.getHeight());

    for (int y = 0; y < image.getHeight(); y++) 
        for(int x = 0; x < image.getWidth(); x++) {
            glm::vec3 newColor = glm::vec3(0);
            for (int j = y - 1; j <= y + 1; j++) 
                for (int i = x - 1; i <= x + 1; i++) {
                    int iTmp = std::clamp(i, 0, image.getWidth() - 1);
                    int jTmp = std::clamp(j, 0, image.getHeight() - 1);
                    newColor += image[iTmp, jTmp] * blurKer[i - (x - 1)][j - (y - 1)];
                }

            res.setData(x, y, newColor);
        }
    res.normalize();
    return res;
}

Image ScalarField::smooth() const {
    glm::mat3 smooth = glm::mat3(1, 1, 1,
                                2, 2, 2,
                                4, 4, 4
    );

    Image res(image.getWidth(), image.getHeight());

    for (int y = 0; y < image.getHeight(); y++) 
        for(int x = 0; x < image.getWidth(); x++) {
            glm::vec3 newColor = glm::vec3(0);
            for (int j = y - 1; j <= y + 1; j++) 
                for (int i = x - 1; i <= x + 1; i++) {
                    int iTmp = std::clamp(i, 0, image.getWidth() - 1);
                    int jTmp = std::clamp(j, 0, image.getHeight() - 1);
                    newColor += image[iTmp, jTmp] * smooth[i - (x - 1)][j - (y - 1)];
                }
                
            res.setData(x, y, newColor);
        }
    res.normalize();
    return res;
}

Image ScalarField::streamArea(const int powStreamArea) const {
    Image streamArea = Image(image);

    int indiceTab = 0;
    using coordonne = std::pair<int, int>;
    auto getCoord = [this](const int i) {
        int x = i % image.getWidth();
        int y = floor(i / image.getWidth());
        return std::make_pair(x, y);
    };
    std::vector<std::pair<float, coordonne>> pixels(image.getData().size()); 
    std::transform(image.getData().begin(), image.getData().end(), pixels.begin(),
                [&indiceTab, &getCoord](const glm::vec3& pixel) {
                    std::pair<float, coordonne>res = std::make_pair(pixel.r, getCoord(indiceTab));
                    ++indiceTab;
                    return res;
                });
    std::sort(pixels.begin(), pixels.end(), 
            [](const auto& left, const auto& right) {
                return left.first > right.first;
            });
    const std::array<int, 8> diffX = {-1, 0, 1,
                                      -1,        1,
                                      -1, 0, 1
                                    };
    const std::array<int, 8> diffY = {-1, -1, -1,
                                      0,          0,
                                      1, 1,   1
                                    };
    const std::array<float, 8> dist = {sqrtf(2), 1,  sqrtf(2),
                                        1,                 1,
                                        sqrtf(2), 1, sqrtf(2)
                                    };

    std::array<float, 8> elevations = {0, 0, 0,
                                       0,        0,
                                       0, 0, 0
                                    };

    std::array<float, 8> deltaHeights = {0, 0, 0,
                                    0,        0,
                                    0, 0, 0
                                    };

    for (const auto& pixel : pixels) {
        coordonne currentCoord = pixel.second;
        float diffTot = 0;
        for (int i = 0; i < 8; i++) {
            int sidePixCoordX = currentCoord.first + diffX[i];
            int sidePixCoordY = currentCoord.second + diffY[i];

            if(sidePixCoordX < 0 || sidePixCoordX >= image.getWidth() ||
                sidePixCoordY < 0 || sidePixCoordY >= image.getHeight()) continue;

            deltaHeights[i] = pixel.first - image[sidePixCoordX, sidePixCoordY].r;
            if(deltaHeights[i] <= 0) continue;

            float diff = powf(deltaHeights[i] / dist[i], powStreamArea);
            elevations[i] = diff;
            diffTot += diff;
        }
        
        for (int i = 0; i < 8; i++) {
            int sidePixCoordX = currentCoord.first + diffX[i];
            int sidePixCoordY = currentCoord.second + diffY[i];

            if(sidePixCoordX < 0 || sidePixCoordX >= image.getWidth() ||
                sidePixCoordY < 0 || sidePixCoordY >= image.getHeight()) continue;
            
            if(deltaHeights[i] <= 0) continue; // skip pour éviter de diviser elevation par un diffTot = 0
            glm::vec3 streamAreaAdding = streamArea[currentCoord.first, currentCoord.second] * (elevations[i] / diffTot);
            
            streamArea.setData(sidePixCoordX, sidePixCoordY,
                                streamArea[sidePixCoordX, sidePixCoordY]
                                + streamAreaAdding);
        }
        elevations = {0, 0, 0,
                      0,        0,
                      0, 0, 0
                    };
    }


    return streamArea;
}
