#include "heightField.hpp"
#include "image.hpp"
#include <algorithm>
#include <fstream>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <ratio>
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
    float height = image[coorPixX, coorPixY].r * (maxMap.z - minMap.z);
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
            res += glm::length(grad);
        }
    }
    return res / nbPixInImage;
}

std::vector<glm::vec3> HeightField::makeVerticies() const {
    std::vector<glm::vec3> res;
    res.reserve(6 * image.getWidth() * image.getHeight());
    for (int y = 0; y < image.getHeight() - 1; y++) {
        for (int x = 0; x < image.getWidth() - 1; x++) {
            res.push_back(glm::vec3(x, heightGrid(x, y), y));
            res.push_back(glm::vec3(x + 1, heightGrid(x + 1, y), y));
            res.push_back(glm::vec3(x, heightGrid(x, y + 1), y + 1));
            res.push_back(glm::vec3(x, heightGrid(x, y + 1), y + 1));
            res.push_back(glm::vec3(x + 1, heightGrid(x + 1, y), y));
            res.push_back(glm::vec3(x + 1, heightGrid(x + 1, y + 1), y + 1));
        }
    }
    return res;
}

std::vector<HeightField::Triangle> HeightField::makeTriangles(const std::vector<glm::vec3>& verticies) const {
    std::vector<Triangle> res;
    res.reserve(verticies.size() / 3);
    for (int i = 0; i < verticies.size(); i+= 3) {
        res.push_back({i, i + 1, i + 2});
    }
    return res;
}

std::vector<glm::vec3> HeightField::makeNormales(const std::vector<Triangle>& triangles, const std::vector<glm::vec3>& verticies) const {
    std::vector<glm::vec3> res;
    res.reserve(triangles.size());
    int index = 0;
    for (const Triangle& triangle : triangles) {
        res.push_back(triangle.normal(verticies, index));
        index++;
    }
    return res;
}

void HeightField::toObj(const std::string& filename) const {
    std::vector<glm::vec3> verticies = makeVerticies();
    std::vector<Triangle> triangles = makeTriangles(verticies);
    std::vector<glm::vec3> normales = makeNormales(triangles, verticies);

    std::ofstream file = std::ofstream(filename);
    if (file.is_open()) {
        file << "# " << filename << "\n#\no " << filename << "\n\n";
        for (const glm::vec3& vertex : verticies) {
            file << "v " << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
        }

        file << '\n';
        for (const glm::vec3& normal : normales) {
            file << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
        }

        file << '\n';

        for (const Triangle& triangle : triangles) {
            file << "f " << triangle.a << "//" << triangle.indexNorm << ' '
            << triangle.b << "//" << triangle.indexNorm << ' '
            << triangle.c << "//" << triangle.indexNorm << '\n';
        }

    }
    file.close();
}