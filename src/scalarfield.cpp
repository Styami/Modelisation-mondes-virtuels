#include "scalarfield.hpp"
#include <algorithm>

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
        glm::mat3 laplacianKer = glm::mat3(1, 1, 1,
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

