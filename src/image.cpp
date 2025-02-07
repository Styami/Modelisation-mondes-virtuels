#include "image.hpp"
#include <algorithm>
#include <cassert>
#include <glm/fwd.hpp>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stb_image_resize.h>


Image::Image() :
    width(0),
    height(0),
    nbChannel(0)
{}

Image::Image(const int width, const int heigth) :
    width(width),
    height(heigth),
    nbChannel(3),
    data(width * height, glm::vec3(0))
{}

Image::Image(const int width, const int height, const glm::vec3& defaultColor) :
    width(width),
    height(height),
    nbChannel(3),
    data(width * height, defaultColor)
{}


Image::Image(const int width, const int heigth, const int nbChannel, const std::vector<glm::vec3>& data) :
    width(width),
    height(heigth),
    nbChannel(nbChannel),
    data(data.begin(), data.end())
{}

Image::Image(const Image& image) :
    width(image.width),
    height(image.height),
    nbChannel(image.nbChannel),
    data(image.data.begin(), image.data.end())
{}

Image& Image::operator=(const Image& other) {
    if (this == &other) return *this;
    
    width = other.width;
    height = other.height;
    nbChannel = other.nbChannel;
    data = other.data;
    
    return *this;
}

Image::Image(const std::string& filename) {
    load(filename);
}

Image::Image(Image&& image) :
    width(std::move(image.width)),
    height(std::move(image.height)),
    nbChannel(std::move(image.nbChannel)),
    data(image.data.begin(), image.data.end())
{
    image.data.clear();
}

Image Image::resize(const int newWidth, const int newHeight) {
    unsigned char* imageChar = new unsigned char[width * height * (nbChannel)];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) 
            for (int z = 0; z < nbChannel; z++) {
                if (z == 3) {
                    imageChar[z + x * nbChannel + (y * width * nbChannel)] = static_cast<unsigned char>(255); 
                    continue;
                }
                imageChar[z + x * nbChannel + (y * width * nbChannel)] = static_cast<unsigned char>((*this)[x, y][z]);
            }
        
    }
    unsigned char* res = new unsigned char [newHeight * newWidth * nbChannel];
    stbir_resize_uint8(imageChar, width, height, width * nbChannel, res, newWidth, newHeight, newWidth * nbChannel, nbChannel);
    std::vector<glm::vec3> newData(newHeight * newWidth);
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            glm::vec3 color;
            for (int z = 0; z < nbChannel; z++) {
                color[z] = static_cast<double>(res[z + x * nbChannel + y * newWidth * nbChannel]);
            }
            newData[x + y * newWidth] = color;
        }
    }
    stbi_image_free(res);
    return Image(newWidth, newHeight, nbChannel, newData);
}

Image Image::normalize() const {
    auto [min, max] = std::minmax_element(data.begin(), data.end(), [](const glm::vec3& a, const glm::vec3& b) {
        return a.r < b.r;
    });
    std::vector<glm::vec3> pixels = std::vector<glm::vec3>(width * height);
    std::transform(data.begin(), data.end(), pixels.begin(),
                    [min, max](const glm::vec3& pixel) {
                        return glm::vec3((pixel - min->r) / (max->r - min->r));
                    });
    return Image(width, height, 3, pixels);
}

Image Image::normalizeStreamArea() const {
    std::vector<glm::vec3> pixels = std::vector<glm::vec3>(width * height);
    std::transform(getData().begin(), getData().end(), pixels.begin(),[](const glm::vec3& pixel) {
        return glm::vec3(powf(pixel.r, 1/4.f));
    });
    Image res = Image(width,
                height,
                3,
                pixels
                );

    return res.normalize();
}

Image Image::normalizeLaplace() const {
    auto [min, max] = std::minmax_element(data.begin(), data.end(), [](const glm::vec3& a, const glm::vec3& b) {
        return a.r < b.r;
    });
    Image res = Image(width, height, glm::vec3(255));
    if(min->r > 0 || max->r < 0) {
        return normalize();
    }

    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            float currentVal = (*this)[i, j].r;
            if(currentVal < 0) {
                // Prend l'inverse de la couleur puis soustrait à la couleur blanche
                float ratio = currentVal / min->r;
                glm::vec3 newPixel = glm::vec3(1) - (glm::vec3(1) - glm::vec3(0, 0, 1)) * ratio;
                res.setData(i, j, newPixel);
            } else {
                float ratio = currentVal / max->r;
                glm::vec3 newPixel = glm::vec3(1) - (glm::vec3(1) - glm::vec3(1, 0, 0)) * ratio;
                res.setData(i, j, newPixel);
            }
        }
    return res;
}

Image Image::normalizeNormGrad() const {
    Image imgToSave = normalize();
    std::vector<glm::vec3> newPixels(width * height);
    std::transform(imgToSave.getData().begin(), imgToSave.getData().end(), newPixels.begin(),
                    [](const glm::vec3& pixel) {
                        return glm::vec3(1) - pixel;
                    });
    return Image(width, height, nbChannel, newPixels);
}

void Image::load(const std::string& filename) {
    unsigned char* res = stbi_load(filename.c_str(), &width, &height, &nbChannel, 3);
    if(res == nullptr) {
        assert(res != nullptr);
    }
    data = std::vector<glm::vec3>(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec3 color;
            for (int z = 0; z < nbChannel; z++) {
                color[z] = static_cast<double>(res[z + x * nbChannel + y * width * nbChannel]/255.f);
            }
            data[x + y * width] = color;
        }
    }
    normalize();
    stbi_image_free(res);
}

void Image::saveStreamArea(const std::string& filename) const {
    Image imgToSave = normalizeStreamArea();
    imgToSave.save(filename);
}

void Image::saveLaplace(const std::string& filename) const {
    Image imgToSave = normalizeLaplace();
    imgToSave.save(filename);
}

void Image::saveNormGrad(const std::string& filename) const {
    Image imgToSave = normalizeNormGrad();
    imgToSave.save(filename);
}

void Image::save(const std::string& filename) const {
    unsigned char* imageChar = new unsigned char[width * height * (nbChannel)];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) 
            for (int z = 0; z < nbChannel; z++) {
                if (z == 3) {
                    imageChar[z + x * nbChannel + (y * width * nbChannel)] = static_cast<unsigned char>(255); 
                    continue;
                }
                float pixel = (*this)[x, y][z] * 255;
                unsigned char newPixel = static_cast<unsigned char>(pixel);
                imageChar[z + x * nbChannel + (y * width * nbChannel)] = newPixel;
            }
        
    }
    stbi_write_png(filename.c_str(), width, height, nbChannel, imageChar, width * nbChannel);
    delete [] imageChar;
}




void Image::setData(const int x, const int y, const glm::vec3& pixel) {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    data[x + y * width] = pixel;
}

Image::~Image() {
    // delete [] data;
}

