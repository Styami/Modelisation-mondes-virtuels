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
    private:
        Image image;
        Image&& scaleData(const std::vector<float>& pixels, Image& res) const;
};

#endif