#ifndef __SCALARFIELD__
#define __SCALARFIELD__
#include "image.hpp"
#include <string>

class ScalarField {
    public:
        ScalarField(const std::string& filename);
        void save(const std::string& filename);
        Image gradient() const;
        Image laplacian() const;
    private:
        Image image;
};

#endif