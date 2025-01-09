#include "heightField.hpp"

HeightField::HeightField(const std::string& filename, const glm::vec3& min, const glm::vec3& max) :
    ScalarField(filename),
    sizeX(image.getWidth()),
    sizeY(image.getHeight()),
    minMap(min),
    maxMap(max)
{
    for (const glm::vec3 vec : image.getData()) {
        heights.push_back(vec.z);
    }
}

HeightField::HeightField(const Image& image, const glm::vec3& min, const glm::vec3& max) :
    ScalarField(image),
    sizeX(image.getWidth()),
    sizeY(image.getHeight()),
    minMap(min),
    maxMap(max)
{}

HeightField::HeightField(HeightField&& heightField) :
    ScalarField(std::move(heightField)),
    sizeX(std::move(heightField.sizeX)),
    sizeY(std::move(heightField.sizeY)),
    minMap(std::move(heightField.minMap)),
    maxMap(std::move(heightField.maxMap))
{}

float HeightField::heightGrid(const int x, const int y) const {
    
    int coorPixX = std::clamp(x, 0, image.getWidth() - 1);
    int coorPixY = std::clamp(y, 0, image.getHeight() - 1);

    // auto [minHeight, maxHeight] = std::minmax_element(heights.begin(), heights.end());
    float normalizeHeight = image[coorPixX, coorPixY].z / 255.f;
    float height = normalizeHeight * (maxMap.z - minMap.z) - minMap.z;
    return height;
}

float HeightField::height(const float x, const float y) const {
    assert(x >= minMap.x || x < maxMap.x);
    assert(y >= minMap.y || y < maxMap.y);

    // permet de connaître la coordonnée normalisée
    float u =  (x - minMap.x ) / (maxMap.x - minMap.x);
    float v =  (y - minMap.y ) / (maxMap.y - minMap.y);

    // permet de connaître la coordonnée du point de la grille
    float nu = u * sizeX;
    float nv = v * sizeY;

    float nuInt;
    u = std::modf(nu, &nuInt);
    float nvInt;
    v = std::modf(nv, &nvInt); 

    // interpolation bilinéaire
    float res = 0;
    float c0 = heightGrid(nuInt, nvInt) * (1 - u) + heightGrid(nuInt + 1, nvInt) * u;
    float c1 = heightGrid(nuInt, nvInt + 1) * (1 - u) + heightGrid(nuInt + 1, nvInt + 1) * u;

    res = c0 * (1 - v) + c1 * v;
    return res;
}

Image HeightField::slope() const {
    return normGradient();
}

float HeightField::averageSlope(const int x, const int y) const {
    int nbPixInImage = 0;
    float res = 0;
    for (int j = y - 1; j <= y + 1; j++) {
        if (j < 0 || j > image.getHeight()) continue;
        for (int i = x - 1; i <= x + 1; i++) {
            if(i < 0 || i > image.getWidth()) continue;
            nbPixInImage++;
            glm::vec2 grad = gradient(i, j);
            res += glm::sqrt(grad.x * grad.x + grad.y * grad.y);
        }
    }
    return res / nbPixInImage;
}