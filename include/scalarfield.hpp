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
        Image gradient() const;
        Image laplacian() const;
        Image blur() const;
        Image smooth() const;
    protected:
        void normalize(const std::vector<float>& pixels, Image& image) const;
        Image&& clamp(Image& image, int valueMin) const;
        Image image;
};

#endif