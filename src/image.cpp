#include "image.hpp"
#include <stb_image_resize.h>
#include <cassert>

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
                color[z] = static_cast<double>(res[z + x * nbChannel + y * width * nbChannel]);
            }
            data[x + y * width] = color;
        }
    }
    stbi_image_free(res);
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
                float tmpf = (*this)[x, y][z];
                unsigned char tmp = static_cast<unsigned char>(tmpf);
                imageChar[z + x * nbChannel + (y * width * nbChannel)] = tmp;
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

