#include "scalarfield.hpp"
#include "image.hpp"
#include <algorithm>
#include <cstdio>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <vector>

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

Image&& ScalarField::scaleData(const std::vector<float>& pixels, Image& res) const {
    auto [min, max] = std::minmax_element(pixels.begin(), pixels.end());
    for (int j = 0; j < res.getHeigth(); j++)
        for (int i = 0; i < res.getWidth(); i++) {
            res.setData(i, j, glm::vec3(255) * (res[i, j] - *min)/(*max - *min));
        }
    return std::move(res);
}

Image ScalarField::gradient() const {
    glm::mat3 gradY = glm::mat3(0, -1, 0,
                    -1, 0, 1,
                    0, 1, 0
    );
    glm::mat3 gradX = glm::mat3(-1, -1, -1,
                    0, 0, 0,
                    1, 1, 1
    );

    Image tmp(image.getWidth(), image.getHeigth());
    std::vector<float> pixels;
    pixels.resize(image.getHeigth() * image.getWidth());

    for (int y = 1; y < image.getHeigth() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColorX(0, 0, 0);
            glm::vec3 newColorY(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColorX += image[i, j] * gradX[i - (x - 1)][j - (y - 1)];
                    newColorY += image[i, j] * gradY[i - (x - 1)][j - (y - 1)];
                }
            
            glm::vec3 gradient = glm::sqrt(newColorX * newColorX + newColorY * newColorX) ;
            pixels.push_back(gradient[0]);
            tmp.setData(x, y,  gradient);
        }
    Image res = scaleData(pixels, tmp);
    return res;
}

Image ScalarField::laplacian() const {
    glm::mat3 laplacianKer = glm::mat3(0, 1, 0,
                                    1, -4, 1,
                                    0, 1, 0
    );

    Image tmp(image.getWidth(), image.getHeigth());
    std::vector<float> pixels;
    pixels.resize(image.getHeigth() * image.getWidth());

    for (int y = 1; y < image.getHeigth() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColor += image[i, j] * laplacianKer[i - (x - 1)][j - (y - 1)];
                }
                
            pixels.push_back(newColor[0]);
            tmp.setData(x, y, newColor);
        }

    Image res = scaleData(pixels, tmp);
    return res;
}

Image ScalarField::blur() const {
        glm::mat3 laplacianKer = glm::mat3(1, 1, 1,
                                    1, 1, 1,
                                    1, 1, 1
    );

    Image tmp(image.getWidth(), image.getHeigth());
    std::vector<float> pixels;
    pixels.resize(image.getHeigth() * image.getWidth());

    for (int y = 1; y < image.getHeigth() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColor += image[i, j] * laplacianKer[i - (x - 1)][j - (y - 1)];
                }
                
            pixels.push_back(newColor[0]);
            tmp.setData(x, y, newColor);
        }

    Image res = scaleData(pixels, tmp);
    return res;
}

