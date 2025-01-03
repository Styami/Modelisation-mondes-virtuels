#include "image.hpp"
#include <cassert>
#include <vector>

Image::Image() :
    width(0),
    heigth(0),
    nbChannel(0),
    data(nullptr)
{}

Image::Image(const int width, const int heigth) :
    width(width),
    heigth(heigth),
    nbChannel(3),
    data(new glm::vec3 [width * heigth])
{
    for (int y = 0; y < heigth; y++)
    for (int x = 0; x < width; x++) {
        setData(x, y, {0, 0, 0});
    }
}

Image::Image(const Image& image) :
    width(image.width),
    heigth(image.heigth),
    nbChannel(image.nbChannel)
{
    data = new glm::vec3 [width * heigth];
    for (int y = 0; y < heigth; y++)
    for (int x = 0; x < width; x++) {
        setData(x, y, image[x, y]);
    }
}

Image::Image(const std::string& filename) {
    load(filename);
}

Image::Image(Image&& image) :
    width(std::move(image.width)),
    heigth(std::move(image.heigth)),
    nbChannel(std::move(image.nbChannel)),
    data(std::move(image.data))
{
    image.data = nullptr;
}

void Image::load(const std::string& filename) {
    unsigned char* res = stbi_load(filename.c_str(), &width, &heigth, &nbChannel, 3);
    data = new glm::vec3[width * heigth];
    for (int y = 0; y < heigth; y++) {
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
    unsigned char* imageChar = new unsigned char[width * heigth * (nbChannel)];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < heigth; y++) 
            for (int z = 0; z < nbChannel; z++) {
                if (z == 3) {
                    imageChar[z + x * nbChannel + (y * width * nbChannel)] = static_cast<unsigned char>(255); 
                    continue;
                }
                imageChar[z + x * nbChannel + (y * width * nbChannel)] = static_cast<unsigned char>((*this)[x, y][z]);
            }
        
    }
    stbi_write_png(filename.c_str(), width, heigth, nbChannel, imageChar, width * nbChannel);
    delete [] imageChar;
}




void Image::setData(const int x, const int y, const glm::vec3& pixel) {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < heigth);
    data[x + y * width] = pixel;
}

Image::~Image() {
    delete [] data;
}

