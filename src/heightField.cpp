#include "heightField.hpp"
#include "image.hpp"
#include "scalarfield.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <vector>


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

    // permet de connaître la coordonnée local dans la cellule de la grille
    // u = u - nu * (maxMap.x - minMap.x) / sizeX;
    // v = v - nv * (maxMap.y - minMap.y) / sizeY;

    // u = std::clamp(u, 0.f, 1.f);
    // v = std::clamp(v, 0.f, 1.f);

    // interpolation bilinéaire
    float res = 0;
    float c0 = heightGrid(nuInt, nvInt) * (1 - u) + heightGrid(nuInt + 1, nvInt) * u;
    float c1 = heightGrid(nuInt, nvInt + 1) * (1 - u) + heightGrid(nuInt + 1, nvInt + 1) * u;

    res = c0 * (1 - v) + c1 * v;
    return res;
}

Image HeightField::slope() const {
    Image res(sizeX, sizeY);
    std::vector<float> pixels;
    pixels.resize(sizeX * sizeY);
    for (int j = 0; j < sizeY; j++)
        for(int i = 0; i < sizeX; i++) {
            pixels[i + sizeX * j] = averageSlope(i, j);
            // pixels[i + sizeX * j] = 255.0;
            
        }
    normalize(pixels, res);
    // ScalarField slope = ScalarField();
    return res;
}

float HeightField::averageSlope(const float x, const float y) const {
    float diff = 0;
    float sumAlt = 0;
                 
    for (float i = x - 1; i <= x + 1; i++) {
        for (float j = y - 1; j <= y + 1; j++) {
            float a = 0;
            float b = 0;
            if(i < minMap.x || j < minMap.y || i > maxMap.x || j > maxMap.y) continue;
            if(i == x && j == y) {
                sumAlt += height(i, j);
                continue;      
            }
            sumAlt += height(i, j);
            diff += (height(i, j) - height(x, y)) / glm::distance(glm::vec2(i, j), glm::vec2(x, y));

        }
    }
    return diff / sumAlt;
}