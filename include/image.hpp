#ifndef __IMAGE__
#define __IMAGE__
#include <string>
#include <vector>
#include "glm/glm.hpp"


class Image {
    public:
        Image();
        Image(const int width, const int heigth);
        Image(const int width, const int heigth, const int nbChannel, const std::vector<glm::vec3>& data);
        Image(const Image& image);
        Image(Image&& image);
        Image(const std::string& filename);
        ~Image();
        void load(const std::string& filename);
        void save(const std::string& filename) const;
        Image resize(const int width, const int height);
        const glm::vec3& operator[](const int x, const int y) const;
        int getWidth() const;
        int getHeight() const;
        int getChannels() const;
        void setData(const int x, const int y, const glm::vec3& pixel);
        const std::vector<glm::vec3>& getData() const; 

    private:
        int width;
        int height;
        int nbChannel;
        std::vector<glm::vec3> data;

};

inline int Image::getWidth() const {
    return width;
}

inline const glm::vec3& Image::operator[](const int x, const int y) const {
    assert(x <= 0 || x < width);
    assert(y <= 0 || y < height);
    return data[x + y * width];
}

inline int Image::getHeight() const {
    return height;
}


inline int Image::getChannels() const {
    return nbChannel;
}


inline const std::vector<glm::vec3>& Image::getData() const {
    return data;
}
#endif