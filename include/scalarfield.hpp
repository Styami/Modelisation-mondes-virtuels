#ifndef __SCALARFIELD__
#define __SCALARFIELD__
#include "image.hpp"
#include <string>
#include <vector>

class ScalarField {
    public:
        ScalarField(const std::string& filename);
        ScalarField(const Image& image);
        ScalarField(ScalarField&& scalarfield);
        void save(const std::string& filename);
        glm::vec3 gradient(const int i, const int j) const;
        Image normGradient() const;
        Image laplacian() const;
        Image blur() const;
        Image smooth() const;
    protected:
        void normalize(const std::vector<float>& pixels, Image& image) const;
        Image&& clamp(Image& image, int valueMin) const;
        Image image;
};

#endif