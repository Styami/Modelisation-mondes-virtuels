#include "scalarfield.hpp"
#include "image.hpp"
#include <cstdio>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <iostream>

ScalarField::ScalarField(const std::string& filename) :
    image(filename)
{}

void ScalarField::save(const std::string& filename) {
    image.save(filename);
}

Image ScalarField::gradient() const {
    glm::mat3 gradY = glm::mat3(-1, 0, 1,
                    -1, 0, 1,
                    -1, 0, 1
    );
    glm::mat3 gradX = glm::mat3(-1, -1, -1,
                    0, 0, 0,
                    1, 1, 1
    );

    Image res(image.getWidth(), image.getHeigth());

    for (int y = 1; y < image.getHeigth() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColorX(0, 0, 0);
            glm::vec3 newColorY(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColorX += image[i, j] * gradX[i - (x - 1)][j - (y - 1)];
                    newColorY += image[i, j] * gradY[i - (x - 1)][j - (y - 1)];
                }
                

            glm::vec3 gradient = glm::sqrt(newColorX * newColorX + newColorY * newColorX);
            res.setData(x, y,  gradient);
        }
    return res;
}

Image ScalarField::laplacian() const {
    glm::mat3 laplacianKer = glm::mat3(0, -1, 0,
                    -1, 4, -1,
                    0, -1, 0
    );

    Image res(image.getWidth(), image.getHeigth());

    for (int y = 1; y < image.getHeigth() - 1; y++) 
        for(int x = 1; x < image.getWidth() - 1; x++) {
            glm::vec3 newColor(0, 0, 0);
            for (int i = x - 1; i <= x + 1; i++)
                for (int j = y - 1; j <= y + 1; j++) {
                    newColor += image[i, j] * laplacianKer[i - (x - 1)][j - (y - 1)];
                }
                

            res.setData(x, y,  newColor);
        }
    return res;
}

