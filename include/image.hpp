#ifndef __IMAGE__
#define __IMAGE__
#include <string>
#include "stb_image.h"
#include "stb_image_write.h"
#include "glm/ext/vector_float3.hpp"


class Image {
    public:
        Image();
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
        const glm::vec3* getData() const; 

    private:
        int width;
        int heigth;
        int nbChannel;
        glm::vec3* data;

};
#endif