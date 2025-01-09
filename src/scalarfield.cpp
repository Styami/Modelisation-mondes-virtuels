#include "scalarfield.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <glm/geometric.hpp>
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

void ScalarField::normalize(const std::vector<float>& pixels, Image& image) const {
    auto [min, max] = std::minmax_element(pixels.begin(), pixels.end());
    for (int j = 0; j < image.getHeight(); j++)
        for (int i = 0; i < image.getWidth(); i++) {
            image.setData(i, j, glm::vec3(255) * (image[i, j] - *min)/(*max - *min));
        }
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
    glm::mat3 gradY = glm::mat3(-1, 0, 1,
                    -1, 0, 1,
                    -1, 0, 1
    );
    glm::mat3 gradX = glm::mat3(-1, -1, -1,
                    0, 0, 0,
                    1, 1, 1
    );

    Image tmp(image.getWidth(), image.getHeight());
    std::vector<float> pixels;
    pixels.resize(image.getHeight() * image.getWidth());

    for (int y = 1; y < image.getHeight() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColorX(0, 0, 0);
            glm::vec3 newColorY(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColorX += image[i, j] * gradX[i - (x - 1)][j - (y - 1)];
                    newColorY += image[i, j] * gradY[i - (x - 1)][j - (y - 1)];
                }
            
            glm::vec3 gradient = glm::sqrt(newColorX * newColorX + newColorY * newColorY) ;
            pixels.push_back(gradient[0]);
            tmp.setData(x, y,  gradient);
        }
    normalize(pixels, tmp);
    return tmp;
}

glm::vec2 ScalarField::gradient(const int x, const int y) const {
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
        if(j < 0 || j > image.getHeight()) continue;
        for (int i = x - 1; i <= x + 1; i++) {
            if(i < 0 || i > image.getWidth()) continue;
            valueX += image[i, j][0] * gradX[i - (x - 1)][j - (y - 1)];
            valueY += image[i, j][0] * gradY[i - (x - 1)][j - (y - 1)];
        }
    }
    return glm::normalize(glm::vec2(valueX, valueY));
}

Image ScalarField::laplacian() const {
    glm::mat3 laplacianKer = glm::mat3(0, 1, 0,
                                    1, -4, 1,
                                    0, 1, 0
    );

    Image tmp(image.getWidth(), image.getHeight());
    std::vector<float> pixels;
    pixels.resize(image.getHeight() * image.getWidth());

    for (int y = 1; y < image.getHeight() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColor += image[i, j] * laplacianKer[i - (x - 1)][j - (y - 1)];
                }
                
            pixels.push_back(newColor[0]);
            tmp.setData(x, y, newColor);
        }

    normalize(pixels, tmp);
    return tmp;
}

Image ScalarField::blur() const {
        glm::mat3 laplacianKer = (1 / 9.0f) * glm::mat3(1, 1, 1,
                                    1, 1, 1,
                                    1, 1, 1
    );

    Image tmp(image.getWidth(), image.getHeight());
    std::vector<float> pixels;
    pixels.resize(image.getHeight() * image.getWidth());

    for (int y = 1; y < image.getHeight() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColor += image[i, j] * laplacianKer[i - (x - 1)][j - (y - 1)];
                }
                
            pixels.push_back(newColor[0]);
            tmp.setData(x, y, newColor);
        }

    normalize(pixels, tmp);
    return tmp;
}

Image ScalarField::smooth() const {
    glm::mat3 smooth = (1/21.f) * glm::mat3(1, 1, 1,
                                    2, 2, 2,
                                    4, 4, 4
    );

    Image tmp(image.getWidth(), image.getHeight());
    std::vector<float> pixels;
    pixels.resize(image.getHeight() * image.getWidth());

    for (int y = 1; y < image.getHeight() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColor += image[i, j] * smooth[i - (x - 1)][j - (y - 1)];
                }
                
            pixels.push_back(newColor[0]);
            tmp.setData(x, y, newColor);
        }

    normalize(pixels, tmp);
    return tmp;
}

Image ScalarField::streamArea(const int powStreamArea) const {
    Image streamArea(image.getWidth(), 
        image.getHeight(),
        3,
        std::vector<glm::vec3>(image.getWidth() * image.getHeight(), glm::vec3(1)));
    int indiceTab = 0;
    using coordonne = std::pair<int, int>;
    std::function<coordonne(int)>  getCoord = [this](const int i) {
        int x = i % image.getWidth();
        int y = (i - x) / image.getWidth();
        return std::make_pair(x, y);
    };
    std::vector<std::pair<float, coordonne>> pixels(image.getData().size()); 
    std::transform(image.getData().begin(), image.getData().end(), pixels.begin(),
                                [&indiceTab, getCoord](const glm::vec3& pixel) {
                                    std::pair<float, coordonne>res = std::make_pair(pixel[0], getCoord(indiceTab));
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
    for (const auto& pixels : pixels) {
        coordonne currentCoord = pixels.second;
        float diffTot = 0;
        for (int i = 0; i < 8; i++) {
            int coordX = currentCoord.first + diffX[i];
            int coordY = currentCoord.second + diffY[i];

            if(coordX < 0 || coordX >= image.getWidth()) continue;
            if(coordY < 0 || coordY >= image.getHeight()) continue;

            float currentDiff = image[coordX, coordY][0] - image[currentCoord.first, currentCoord.second][0];
            if(currentDiff >= 0) continue;

            diffTot += currentDiff / dist[i];
        }
        diffTot = powf(diffTot, powStreamArea);
        for (int i = 0; i < 8; i++) {
            int coordX = currentCoord.first + diffX[i];
            int coordY = currentCoord.second + diffY[i];

            if(coordX < 0 || coordX >= image.getWidth()) continue;
            if(coordY < 0 || coordY >= image.getHeight()) continue;

            float currentDiff = image[coordX, coordY][0] - image[currentCoord.first, currentCoord.second][0];
            if(currentDiff >= 0) continue;

            streamArea.setData(coordX, coordY,
                                streamArea[coordX, coordY] + streamArea[currentCoord.first, currentCoord.second] * powf(currentDiff / dist[i], powStreamArea) / diffTot);
            streamArea.setData(coordX, coordY,
                                glm::vec3(powf(streamArea[coordX, coordY][0], 1/4.0f)));
        }
    }

    std::vector<float> valuesStreamArea(image.getWidth() * image.getHeight());
    std::transform(streamArea.getData().begin(), streamArea.getData().end(), valuesStreamArea.begin(),
                    [](const auto& p) {
                        return p[0];
                    });
    normalize(valuesStreamArea, streamArea);
    return streamArea;
}
