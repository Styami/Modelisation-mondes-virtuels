#ifndef __IMAGE__
#define __IMAGE__
#include <string>
#include "stb_image.h"
#include "stb_image_write.h"
#include "glm/glm.hpp"


class Image {
    public:
        Image();
        Image(const int width, const int heigth);
        Image(const Image& image);
        Image(Image&& image);
        Image(const std::string& filename);
        ~Image();
        void load(const std::string& filename);
        void save(const std::string& filename) const;
        const glm::vec3& operator[](const int x, const int y) const;
        int getWidth() const;
        int getHeigth() const;
        int getChannels() const;
        void setData(const int x, const int y, const glm::vec3& pixel);
        const glm::vec3* getData() const; 

    private:
        int width;
        int heigth;
        int nbChannel;
        glm::vec3* data;

};

inline int Image::getWidth() const {
    return width;
}

inline const glm::vec3& Image::operator[](const int x, const int y) const {
    return data[x + y * width];
}

inline int Image::getHeigth() const {
    return heigth;
}


inline int Image::getChannels() const {
    return nbChannel;
}


inline const glm::vec3* Image::getData() const {
    return data;
}
#endif